//
// Created by 58413 on 2023/10/16.
//

#include "BindGroupLayoutBuilder.h"

namespace frieren_core {
    BindGroupLayoutBuilder::BindGroupLayoutBuilder(const string& name) {
        this->name = name;
    }

    void set_default(WGPUBindGroupLayoutEntry& e) {
        e.buffer.nextInChain = nullptr;
        e.buffer.type = WGPUBufferBindingType_Undefined;
        e.buffer.hasDynamicOffset = false;

        e.sampler.nextInChain = nullptr;
        e.sampler.type = WGPUSamplerBindingType_Undefined;

        e.storageTexture.nextInChain = nullptr;
        e.storageTexture.access = WGPUStorageTextureAccess_Undefined;
        e.storageTexture.format = WGPUTextureFormat_Undefined;
        e.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

        e.texture.nextInChain = nullptr;
        e.texture.multisampled = false;
        e.texture.sampleType = WGPUTextureSampleType_Undefined;
        e.texture.viewDimension = WGPUTextureViewDimension_Undefined;
    }

    BindGroupLayoutBuilder &BindGroupLayoutBuilder::add_uniform_buffer(int binding) {
        WGPUBindGroupLayoutEntry e;

        e.nextInChain = nullptr;
        set_default(e);

        e.binding = binding;
        e.visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;

        e.buffer.type = WGPUBufferBindingType_Uniform;
        e.buffer.hasDynamicOffset = false;
        e.buffer.minBindingSize = 0;

        entries.push_back(e);

        return *this;
    }

    BindGroupLayoutBuilder& BindGroupLayoutBuilder::add_sampler(int binding, WGPUSamplerBindingType bindingType) {
        WGPUBindGroupLayoutEntry e;

        e.nextInChain = nullptr;
        set_default(e);

        e.binding = binding;
        e.visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;

        e.sampler.type = bindingType;
        e.sampler.nextInChain = nullptr;

        entries.push_back(e);

        return *this;
    }

    BindGroupLayoutBuilder& BindGroupLayoutBuilder::add_texture(int binding, WGPUTextureViewDimension dim) {
        WGPUBindGroupLayoutEntry e;

        e.nextInChain = nullptr;
        set_default(e);

        e.binding = binding;
        e.visibility = WGPUShaderStage_Vertex | WGPUShaderStage_Fragment;

        e.texture.sampleType = WGPUTextureSampleType_Float;
        e.texture.viewDimension = dim;

        return *this;
    }

    WGPUBindGroupLayout BindGroupLayoutBuilder::build(WGPUDevice device) const {
        WGPUBindGroupLayoutDescriptor desc{};
        desc.nextInChain = nullptr;
        desc.label = name.c_str();
        desc.entryCount = entries.size();
        desc.entries = entries.data();
        WGPUBindGroupLayout layout = wgpuDeviceCreateBindGroupLayout(device, &desc);

        return layout;
    }
}
