//
// Created by 58413 on 2023/10/16.
//

#ifndef FRIEREN_BINDGROUPBUILDER_H
#define FRIEREN_BINDGROUPBUILDER_H

#include <vector>
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    class BindGroupBuilder {
    private:
        vector<WGPUBindGroupEntry> entries;
    public:
        BindGroupBuilder() = default;

        BindGroupBuilder& add_uniform_buffer(int binding, WGPUBuffer buffer, int offset, int size);

        BindGroupBuilder& add_whole_buffer(int binding, WGPUBuffer buffer);

        BindGroupBuilder& add_texture(int binding, WGPUTextureView texture_view);

        BindGroupBuilder& add_sampler(int binding, WGPUSampler sampler);

        WGPUBindGroup build(WGPUDevice device, WGPUBindGroupLayout layout) const;
    };
}



#endif //FRIEREN_BINDGROUPBUILDER_H
