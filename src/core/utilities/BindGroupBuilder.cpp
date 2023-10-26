//
// Created by 58413 on 2023/10/16.
//

#include "BindGroupBuilder.h"

namespace frieren_core {
    BindGroupBuilder &BindGroupBuilder::add_uniform_buffer(int binding, WGPUBuffer buffer, int offset, int size) {
        WGPUBindGroupEntry e;

        e.nextInChain = nullptr;
        e.binding = binding;
        e.buffer = buffer;
        e.offset = offset;
        e.size = size;

        entries.push_back(e);

        return *this;
    }

    BindGroupBuilder& BindGroupBuilder::add_whole_buffer(int binding, WGPUBuffer buffer) {
        WGPUBindGroupEntry e;

        e.nextInChain = nullptr;
        e.binding = binding;
        e.buffer = buffer;
        e.size = wgpuBufferGetSize(buffer);
        e.offset = 0;

        entries.push_back(e);

        return *this;
    }

    BindGroupBuilder& BindGroupBuilder::add_texture(int binding, WGPUTextureView texture_view) {
        WGPUBindGroupEntry e;

        e.nextInChain = nullptr;
        e.binding = binding;
        e.textureView = texture_view;

        entries.push_back(e);

        return *this;
    }

    WGPUBindGroup BindGroupBuilder::build(WGPUDevice device, WGPUBindGroupLayout layout) const {
        WGPUBindGroupDescriptor desc;

        desc.nextInChain = nullptr;
        desc.layout = layout;
        desc.entryCount = entries.size();
        desc.entries = entries.data();

        return wgpuDeviceCreateBindGroup(device, &desc);
    }
}
