#ifndef FRIEREN_CORE_TEXTURE_H
#define FRIEREN_CORE_TEXTURE_H

#include <webgpu/webgpu.hpp>
#include <common_include.h>

using namespace std;

namespace frieren_core {
    struct TextureDescriptor {
        string name;
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

        void* cpu_data = nullptr;
        WGPUExtent3D texture_size;
        int pixel_size;

        WGPUTexture texture;
        WGPUTextureView texture_view;
    public:
        Texture(WGPUTexture texture, WGPUTextureView texture_view);
        explicit Texture(WGPUDevice device, const TextureDescriptor& texture_desc);
        ~Texture();

        WGPUTextureView get_wgpu_texture_view() const;

        void write_texture(WGPUQueue queue) const;
    };
}

#endif