#include <pbr_multi_bounce/PBRMultiBounce.h>
#include <common_include_glm.h>
#include <stb_image.h>
#include <stb_image_write.h>
#include <iostream>
#include <texture/MyTextureFormat.h>
#include <webgpu/webgpu.hpp>

using namespace std;
using namespace frieren_precompute;
using namespace frieren_core;

int main() {
    PBRMultiBounce precompute;
    const int size = 256;
    const int channel = 1;

    uint8_t data[size * size * channel];
    precompute.generate_map(size, data);
    stbi_write_bmp("r_sf1.bmp", size, size, channel, data);
    MyTextureFormatData format_data;
    format_data.header.pixel_bytes = 1;
    format_data.header.depth = 1;
    format_data.header.width = size;
    format_data.header.height = size;
    format_data.header.mip_level_count = 1;
    format_data.header.desired_format = WGPUTextureFormat_R8Unorm;
    format_data.data.assign(data, data + size * size * channel);
    MyTextureFormat::write_texture("r_sf1.frieren_texture", format_data);

     uint8_t r_sf1_bar_map[size * channel];
     precompute.generate_R_sf1_bar_map(size, r_sf1_bar_map);
     stbi_write_bmp("r_sf1_bar.bmp", size, 1, channel, r_sf1_bar_map);
     format_data.header.pixel_bytes = 1;
     format_data.header.depth = 1;
     format_data.header.width = size;
     format_data.header.height = 1;
     format_data.header.mip_level_count = 1;
     format_data.header.desired_format = WGPUTextureFormat_R8Unorm;
     format_data.data.clear();
     format_data.data.assign(r_sf1_bar_map, r_sf1_bar_map + size * channel);
     MyTextureFormat::write_texture("r_sf1_bar.frieren_texture", format_data);

    return 0;
}