#include "utils.h"

namespace frieren_core::utils {
    optional<string> read_file_to_string(const std::filesystem::path& path) {
        std::ifstream t(path);

        if (!t.is_open()) {
            return {};
        }

        std::stringstream buffer;
        buffer << t.rdbuf();

        return std::move(buffer.str());
    }

    string get_json_object_any_key(const json& j) {
        assert(j.is_object());
        for (const auto& p: j.items()) {
            return p.key();
        }
        assert(false);
        return "";
    }

    int get_wgpu_texture_format_pixel_size(WGPUTextureFormat format) {
        switch (format) {
            case WGPUTextureFormat_Undefined:
                assert(false);
                return 0;
            case WGPUTextureFormat_R8Unorm:
                return 1;
            case WGPUTextureFormat_R8Snorm:
                return 1;
            case WGPUTextureFormat_R8Uint:
                return 1;
            case WGPUTextureFormat_R8Sint:
                return 1;
            case WGPUTextureFormat_R16Uint:
                return 2;
            case WGPUTextureFormat_R16Sint:
                return 2;
            case WGPUTextureFormat_R16Float:
                return 2;
            case WGPUTextureFormat_RG8Unorm:
                return 2;
            case WGPUTextureFormat_RG8Snorm:
                return 2;
            case WGPUTextureFormat_RG8Uint:
                return 2;
            case WGPUTextureFormat_RG8Sint:
                return 2;
            case WGPUTextureFormat_R32Float:
                return 4;
            case WGPUTextureFormat_R32Uint:
                return 4;
            case WGPUTextureFormat_R32Sint:
                return 4;
            case WGPUTextureFormat_RG16Uint:
                return 4;
            case WGPUTextureFormat_RG16Sint:
                return 4;
            case WGPUTextureFormat_RG16Float:
                return 4;
            case WGPUTextureFormat_RGBA8Unorm:
                return 4;
            case WGPUTextureFormat_RGBA8UnormSrgb:
                return 4;
            case WGPUTextureFormat_RGBA8Snorm:
                return 4;
            case WGPUTextureFormat_RGBA8Uint:
                return 4;
            case WGPUTextureFormat_RGBA8Sint:
                return 4;
            case WGPUTextureFormat_BGRA8Unorm:
                return 4;
            case WGPUTextureFormat_BGRA8UnormSrgb:
                return 4;
            case WGPUTextureFormat_RGB10A2Unorm:
                return 4;
            case WGPUTextureFormat_RG11B10Ufloat:
                return 4;
            case WGPUTextureFormat_RGB9E5Ufloat:
                return 4;
            case WGPUTextureFormat_RG32Float:
                return 4;
            case WGPUTextureFormat_RG32Uint:
                return 8;
            case WGPUTextureFormat_RG32Sint:
                return 8;
            case WGPUTextureFormat_RGBA16Uint:
                return 8;
            case WGPUTextureFormat_RGBA16Sint:
                return 8;
            case WGPUTextureFormat_RGBA16Float:
                return 8;
            case WGPUTextureFormat_RGBA32Float:
                return 16;
            case WGPUTextureFormat_RGBA32Uint:
                return 16;
            case WGPUTextureFormat_RGBA32Sint:
                return 16;
            case WGPUTextureFormat_Stencil8:
                return 1;
            case WGPUTextureFormat_Depth16Unorm:
                return 2;
            case WGPUTextureFormat_Depth24Plus:
                return 3;
            case WGPUTextureFormat_Depth24PlusStencil8:
                return 4;
            case WGPUTextureFormat_Depth32Float:
                return 4;
            case WGPUTextureFormat_Depth32FloatStencil8:
                return 5;
            default:
                assert(false);
                return 0;
        }
    }
}
