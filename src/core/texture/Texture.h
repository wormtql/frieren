#ifndef FRIEREN_CORE_TEXTURE_H
#define FRIEREN_CORE_TEXTURE_H

#include <webgpu/webgpu.hpp>
#include <common_include.h>

using namespace std;

namespace frieren_core {
    struct TextureDescriptor {
        string name;
        string id;
        string texture_data_path;
        WGPUTextureDimension dimension;
        WGPUTextureViewDimension texture_view_dimension;
        WGPUTextureFormat format;
        uint32_t mip_level_count;
    };

    void from_json(const json& j, TextureDescriptor& desc);

    class Texture {
    private:
        static set<string> texture_ext_stbi;

        string id;
        void* cpu_data = nullptr;
        WGPUExtent3D texture_size{};
        /**
         * how many bytes a pixel takes.
         * For example, a RGBA8 texture takes 4 bytes per pixel
         * a R8 texture takes 1 byte per pixel
         * a Depth24Stencil8 texture takes 4 bytes per pixel
         */
        int pixel_size{};

        WGPUTexture texture{};
        WGPUTextureView texture_view{};
    public:
        static Texture create_depth_stencil_texture(WGPUDevice device, WGPUTextureFormat format, const string& name, int width, int height);

        Texture(WGPUTexture texture, WGPUTextureView texture_view);
        explicit Texture(WGPUDevice device, const TextureDescriptor& texture_desc);
        Texture(WGPUDevice device, const string& name, int width, int height, int pixel_size, WGPUTextureFormat format, WGPUTextureUsage usage);
        Texture(const Texture& other) = delete;
        Texture(Texture&& other) noexcept;
        ~Texture();

        [[nodiscard]] WGPUTextureView get_wgpu_texture_view() const;

        void write_texture(WGPUQueue queue) const;

        [[nodiscard]] const string& get_id() const {
            return id;
        }

        [[nodiscard]] int get_width() const {
            return texture_size.width;
        }

        [[nodiscard]] int get_height() const {
            return texture_size.height;
        }
    };
}

#endif