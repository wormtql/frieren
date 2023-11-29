#include "Texture.h"
#include <stb_image.h>
#include <nanoid/nanoid.h>
#include <wgpu_serde.h>
#include <utilities/utils.h>
#include "MyTextureFormat.h"

namespace frieren_core {
    Texture::Texture(WGPUTexture texture, WGPUTextureView texture_view)
        : texture(texture), texture_view(texture_view)
    {}

    set<string> Texture::texture_ext_stbi{
        "png",
        "jpg",
        "jpeg",
        "tga",
        "bmp",
        "psd",
        "gif",
        "hdr",
        "pic",
    };

    Texture::Texture(WGPUDevice device, const string& name, int width, int height, int pixel_size, WGPUTextureFormat format, int usage) {
        this->texture_size = WGPUExtent3D {
            .width = (unsigned int) width,
            .height = (unsigned int) height,
            .depthOrArrayLayers = 1
        };
        this->pixel_size = pixel_size;
        this->id = nanoid::generate();

        WGPUTextureDescriptor wgpu_texture_desc{};
        wgpu_texture_desc.nextInChain = nullptr;
        wgpu_texture_desc.label = name.c_str();
        wgpu_texture_desc.usage = usage;
        wgpu_texture_desc.dimension = WGPUTextureDimension_2D;
        wgpu_texture_desc.format = format;
        wgpu_texture_desc.mipLevelCount = 1;
        wgpu_texture_desc.sampleCount = 1;
        wgpu_texture_desc.viewFormatCount = 0;
        wgpu_texture_desc.viewFormats = nullptr;
        wgpu_texture_desc.size = this->texture_size;

        this->texture = wgpuDeviceCreateTexture(device, &wgpu_texture_desc);

        WGPUTextureViewDescriptor texture_view_desc{};
        texture_view_desc.nextInChain = nullptr;
        texture_view_desc.label = (name + "_view").c_str();
        texture_view_desc.format = format;
        texture_view_desc.dimension = WGPUTextureViewDimension_2D;
        texture_view_desc.baseMipLevel = 0;
        texture_view_desc.mipLevelCount = 1;
        texture_view_desc.baseArrayLayer = 0;
        texture_view_desc.arrayLayerCount = 1;
        texture_view_desc.aspect = WGPUTextureAspect_All;

        this->texture_view = wgpuTextureCreateView(this->texture, &texture_view_desc);
    }

    Texture::Texture(WGPUDevice device, const TextureDescriptor& texture_desc) {
        WGPUTextureDescriptor wgpu_texture_desc{};
        wgpu_texture_desc.nextInChain = nullptr;
        wgpu_texture_desc.label = texture_desc.name.c_str();
        wgpu_texture_desc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;
        wgpu_texture_desc.dimension = texture_desc.dimension;
        wgpu_texture_desc.format = texture_desc.format;
        wgpu_texture_desc.mipLevelCount = texture_desc.mip_level_count;
        wgpu_texture_desc.sampleCount = 1;
        wgpu_texture_desc.viewFormatCount = 0;
        wgpu_texture_desc.viewFormats = nullptr;

        auto path = std::filesystem::path{texture_desc.texture_data_path};
        if (!filesystem::exists(path)) {
            assert(false);
        }

        string abs_path = filesystem::absolute(path).string();
        filesystem::path filename = path.filename();
        string ext = filename.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

        if (texture_ext_stbi.find(ext) != texture_ext_stbi.end()) {
            int width, height, channels;
            unsigned char* img = stbi_load(abs_path.c_str(), &width, &height, &channels, 0);
            if (img == nullptr) {
                cerr << "cannot load image `" << abs_path << "`" << endl;
                assert(false);
            }

            this->cpu_data.assign(img, img + width * height * channels);
//            this->cpu_data = img;
            // assume each channel takes 1 byte
            this->pixel_size = channels;

            wgpu_texture_desc.size = WGPUExtent3D {
                .width = static_cast<unsigned int>(width),
                .height = static_cast<unsigned int>(height),
                .depthOrArrayLayers = 1
            };
        } else if (ext == ".frieren_texture") {
            MyTextureFormatData data = MyTextureFormat::read_texture(path).value();
            this->cpu_data.assign(data.data.begin(), data.data.end());
            this->pixel_size = data.header.pixel_bytes;
            wgpu_texture_desc.size = WGPUExtent3D {
                .width = static_cast<unsigned int>(data.header.width),
                .height = static_cast<unsigned int>(data.header.height),
                .depthOrArrayLayers = static_cast<unsigned int>(data.header.depth)
            };
        }
        else {
            assert(false);
        }

        this->texture_size = wgpu_texture_desc.size;
        this->texture = wgpuDeviceCreateTexture(device, &wgpu_texture_desc);
        this->id = texture_desc.id;

        WGPUTextureViewDescriptor texture_view_desc{};
        texture_view_desc.nextInChain = nullptr;
        string view_name = texture_desc.name + "_view";
        texture_view_desc.label = view_name.c_str();
        texture_view_desc.format = texture_desc.format;
        texture_view_desc.dimension = texture_desc.texture_view_dimension;
        texture_view_desc.baseMipLevel = 0;
        texture_view_desc.mipLevelCount = 1;
        texture_view_desc.baseArrayLayer = 0;
        texture_view_desc.arrayLayerCount = 1;
        texture_view_desc.aspect = WGPUTextureAspect_All;

        this->texture_view = wgpuTextureCreateView(this->texture, &texture_view_desc);
    }

    Texture::Texture(Texture&& other) noexcept {
        this->id = other.id;
        this->cpu_data = other.cpu_data;
        this->texture_size = other.texture_size;
        this->pixel_size = other.pixel_size;
        this->texture = other.texture;
        this->texture_view = other.texture_view;

        other.texture = nullptr;
        other.texture_view = nullptr;
    }

    Texture::~Texture() {
        if (this->texture_view) {
            wgpuTextureViewRelease(this->texture_view);
            this->texture_view = nullptr;
        }
        if (this->texture) {
            wgpuTextureDestroy(this->texture);
            wgpuTextureRelease(this->texture);
            this->texture = nullptr;
        }
    }

    WGPUTextureView Texture::get_wgpu_texture_view() const {
        return texture_view;
    }

    void Texture::write_texture(WGPUQueue queue) const {
        WGPUImageCopyTexture dest;
        dest.nextInChain = nullptr;
        dest.texture = this->texture;
        dest.mipLevel = 0;
        dest.origin = WGPUOrigin3D { 0, 0, 0 };
        dest.aspect = WGPUTextureAspect_All;

        WGPUTextureDataLayout source;
        source.nextInChain = nullptr;
        source.offset = 0;
        source.bytesPerRow = pixel_size * texture_size.width;
        source.rowsPerImage = texture_size.height;

        int image_size = texture_size.height * texture_size.width * texture_size.depthOrArrayLayers * pixel_size;
        wgpuQueueWriteTexture(queue, &dest, this->cpu_data.data(), image_size, &source, &texture_size);
    }

    Texture Texture::create_depth_stencil_texture(WGPUDevice device, WGPUTextureFormat format, const string &name,
                                                  int width, int height) {
        int pixel_size = utils::get_wgpu_texture_format_pixel_size(format);

        Texture tex{device, name, width, height, pixel_size, format, WGPUTextureUsage_RenderAttachment};
        return move(tex);
    }

    Texture Texture::create_same_as_swap_chain(WGPUDevice device, const string& name, const WGPUSwapChainDescriptor& swap_chain_desc) {
        int pixel_size = utils::get_wgpu_texture_format_pixel_size(swap_chain_desc.format);

        Texture tex{
            device,
            name,
            (int) swap_chain_desc.width,
            (int) swap_chain_desc.height,
            pixel_size,
            swap_chain_desc.format,
            WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_TextureBinding
        };
        return move(tex);
    }
}

// serde
namespace frieren_core {
    void from_json(const json& j, TextureDescriptor& desc) {
        desc.name = j["name"];
        desc.id = j["id"];
        desc.texture_data_path = j["texture_data_path"];
        desc.dimension = j["dimension"].template get<WGPUTextureDimension>();
        desc.texture_view_dimension = j["texture_view_dimension"].template get<WGPUTextureViewDimension>();
        desc.format = j["format"].template get<WGPUTextureFormat>();
        desc.mip_level_count = j["mip_level_count"];
    }
}