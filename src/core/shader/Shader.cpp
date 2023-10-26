//
// Created by 58413 on 2023/10/16.
//

#include "Shader.h"
#include "wgpu_serde.h"

namespace frieren_core {
    void from_json(const json& j, ShaderDescriptor& shader_desc) {
        shader_desc.shader_source_path = j["shader_source_path"];
        shader_desc.shader_source = "";
        shader_desc.name = j["name"];

        shader_desc.primitive_state = j["primitive_state"].template get<WGPUPrimitiveState>();
        shader_desc.depth_stencil_state = j["depth_stencil_state"].template get<WGPUDepthStencilState>();
        for (auto& c: j["color_target_states"]) {
            shader_desc.color_target_states.push_back(c.template get<WGPUColorTargetStateOwned>());
        }
        shader_desc.multisample_state = j["multisample_state"].template get<WGPUMultisampleState>();

        shader_desc.is_instancing = j["is_instancing"];
        
        shader_desc.bind_group_layout_entries = {};
        
    }

    // Shader::Shader(WGPUDevice device, const WGPUShaderModuleDescriptor &desc, const ShaderPipelineKey& shader_pipeline_key)
    //     : shader_pipeline_key(shader_pipeline_key) {
    //     auto code_desc = (WGPUShaderModuleWGSLDescriptor*) desc.nextInChain;
    //     this->shader_source = code_desc->code;
    //     this->shader_module = wgpuDeviceCreateShaderModule(device, &desc);
    // }

    Shader::Shader(WGPUDevice device, const ShaderDescriptor& desc) {
        WGPUShaderModuleDescriptor shader_module_desc;
        shader_module_desc.label = desc.name.c_str();
        WGPUShaderModuleWGSLDescriptor wgsl_desc;
        wgsl_desc.chain.next = nullptr;
        wgsl_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
        wgsl_desc.code = desc.shader_source.c_str();
        shader_module_desc.nextInChain = &wgsl_desc.chain;

        this->shader_source = desc.shader_source;
        this->shader_module = wgpuDeviceCreateShaderModule(device, &shader_module_desc);
        this->name = desc.name;
        this->instancing = desc.is_instancing;

        WGPUBindGroupLayoutDescriptor bind_group_layout_descriptor;
        bind_group_layout_descriptor.nextInChain = nullptr;
        bind_group_layout_descriptor.entryCount = desc.bind_group_layout_entries.size();
        bind_group_layout_descriptor.entries = desc.bind_group_layout_entries.data();
        bind_group_layout_descriptor.label = (desc.name + "_bindGroupLayout").c_str();

        WGPUBindGroupLayout bind_group_layout = wgpuDeviceCreateBindGroupLayout(device, &bind_group_layout_descriptor);

        WGPUPipelineLayoutDescriptor pipeline_layout_descriptor;
        pipeline_layout_descriptor.nextInChain = nullptr;
        pipeline_layout_descriptor.label = (desc.name + "_pipelineLayout").c_str();
        // todo
        vector<WGPUBindGroupLayout> bind_group_layouts;
        bind_group_layouts.push_back(bind_group_layout);
        pipeline_layout_descriptor.bindGroupLayoutCount = 1;
        pipeline_layout_descriptor.bindGroupLayouts = bind_group_layouts.data();

        WGPUPipelineLayout pipeline_layout = wgpuDeviceCreatePipelineLayout(device, &pipeline_layout_descriptor);

        WGPURenderPipelineDescriptor render_pipeline_descriptor;
        render_pipeline_descriptor.nextInChain = nullptr;
        render_pipeline_descriptor.vertex.bufferCount = 1;
        // todo
        render_pipeline_descriptor.vertex.buffers = nullptr;
        render_pipeline_descriptor.vertex.module = shader_module;
        render_pipeline_descriptor.vertex.entryPoint = "vs_main";
        render_pipeline_descriptor.vertex.constantCount = 0;
        render_pipeline_descriptor.vertex.constants = nullptr;
        render_pipeline_descriptor.primitive = desc.primitive_state;
        WGPUFragmentState fragment_state;
        fragment_state.module = shader_module;
        fragment_state.entryPoint = "fs_main";
        fragment_state.constantCount = 0;
        fragment_state.constants = nullptr;
        fragment_state.nextInChain = nullptr;
        fragment_state.targetCount = desc.color_target_states.size();
        vector<WGPUColorTargetState> color_target_states;
        for (const auto& cs: desc.color_target_states) {
            color_target_states.push_back(cs.get_wgpu_state());
        }
        fragment_state.targets = color_target_states.data();
        render_pipeline_descriptor.fragment = &fragment_state;
        render_pipeline_descriptor.depthStencil = &desc.depth_stencil_state;
        render_pipeline_descriptor.multisample = desc.multisample_state;
        render_pipeline_descriptor.layout = pipeline_layout;

        this->render_pipeline = wgpuDeviceCreateRenderPipeline(device, &render_pipeline_descriptor);
    }
}