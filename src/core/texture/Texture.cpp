#include "Texture.h"
#include <stb_image.h>

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

    Texture::Texture(WGPUDevice device, const TextureDescriptor& texture_desc) {
        auto path = std::filesystem::path{texture_desc.texture_data_path};
        if (!filesystem::exists(path)) {
            assert(false);
        }

        string abs_path = filesystem::absolute(path).string();
        filesystem::path filename = path.filename();
        string ext = filename.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });

        WGPUTextureDescriptor wgpu_texture_desc;
        wgpu_texture_desc.nextInChain = nullptr;
        wgpu_texture_desc.label = texture_desc.name.c_str();
        wgpu_texture_desc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;
        wgpu_texture_desc.dimension = texture_desc.dimension;
        wgpu_texture_desc.format = texture_desc.format;
        wgpu_texture_desc.mipLevelCount = texture_desc.mip_level_count;
        wgpu_texture_desc.sampleCount = 1;
        wgpu_texture_desc.viewFormatCount = 0;
        wgpu_texture_desc.viewFormats = nullptr;

        if (texture_ext_stbi.find(ext) != texture_ext_stbi.end()) {
            int width, height, channels;
            unsigned char* img = stbi_load(abs_path.c_str(), &width, &height, &channels, 0);
            if (img == nullptr) {
                cerr << "cannot load image `" << abs_path << "`" << endl;
                assert(false);
            }

            this->cpu_data = img;

            wgpu_texture_desc.size = WGPUExtent3D {
                .width = static_cast<unsigned int>(width),
                .height = static_cast<unsigned int>(height),
                .depthOrArrayLayers = 1
            };
        } else {
            assert(false);
        }
        
        this->texture = wgpuDeviceCreateTexture(device, &wgpu_texture_desc);

        WGPUTextureViewDescriptor texture_view_desc;
        texture_view_desc.nextInChain = nullptr;
        texture_view_desc.label = (texture_desc.name + "_view").c_str();
        texture_view_desc.format = texture_desc.format;
        texture_view_desc.dimension = texture_desc.texture_view_dimension;
        texture_view_desc.baseMipLevel = 0;
        texture_view_desc.mipLevelCount = 1;
        texture_view_desc.baseArrayLayer = 0;
        texture_view_desc.arrayLayerCount = 1;
        texture_view_desc.aspect = WGPUTextureAspect_All;

        this->texture_view = wgpuTextureCreateView(this->texture, &texture_view_desc);
    }

    Texture::~Texture() {
        if (this->texture_view) {
            wgpuTextureViewRelease(this->texture_view);
            this->texture_view = nullptr;
        }
        if (this->texture) {
            wgpuTextureRelease(this->texture);
            wgpuTextureDestroy(this->texture);
            this->texture = nullptr;
        }

        if (this->cpu_data != nullptr) {
            stbi_image_free(this->cpu_data);
            cpu_data = nullptr;
        }
    }

    WGPUTextureView Texture::get_wgpu_texture_view() const {
        return texture_view;
    }
}