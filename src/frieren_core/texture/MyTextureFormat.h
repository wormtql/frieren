#ifndef FRIEREN_MYTEXTUREFORMAT_H
#define FRIEREN_MYTEXTUREFORMAT_H

#include <webgpu/webgpu.hpp>
#include <memory>
#include <vector>
#include <filesystem>
#include <optional>

using namespace std;

namespace frieren_core {
    struct MyTextureFormatHeader {
        int width;
        int height;
        int depth;
        int pixel_bytes;
        WGPUTextureFormat desired_format;
        int mip_level_count;
    };

    struct MyTextureFormatData {
        MyTextureFormatHeader header;
        vector<uint8_t> data;
        vector<pair<int, int>> mips;
    };

    class MyTextureFormat {
    public:
        static optional<MyTextureFormatData> read_texture(const filesystem::path& path);

        static void write_texture(const filesystem::path& path, const MyTextureFormatData& data);
    };
}

#endif //FRIEREN_MYTEXTUREFORMAT_H
