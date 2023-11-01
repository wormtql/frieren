//
// Created by 58413 on 2023/10/16.
//

#include "Shader.h"
#include "wgpu_serde.h"
#include <rendering/BuiltinBindGroupLayout.h>
#include <mesh/Vertex.h>

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
        
        // this is shader-specific bind group layout entries
        for (const auto& bind_group_layout_entry_json: j["bind_entries"]) {
            shader_desc.bind_group_layout_entries.push_back(
                bind_group_layout_entry_json.template get<MyWGPUBindGroupLayoutEntry>()
            );
        }

        shader_desc.shader_property_layout_builder = j["shader_properties"].template get<ShaderPropertyLayoutBuilder>();        
    }

    void from_json(const json& j, MyWGPUBindGroupLayoutEntry& e) {
        e.name = j["name"];
        e.binding = j["binding"];
        e.visibility = j["visibility"].template get<WGPUShaderStageFlags>();

        e.buffer.type = WGPUBufferBindingType_Undefined;
        e.sampler.type = WGPUSamplerBindingType_Undefined;
        e.texture.sampleType = WGPUTextureSampleType_Undefined;
        e.texture.viewDimension = WGPUTextureViewDimension_Undefined;
        e.storageTexture.access = WGPUStorageTextureAccess_Undefined;
        e.storageTexture.format = WGPUTextureFormat_Undefined;
        e.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

        if (j.contains("buffer")) {
            e.buffer = j["buffer"].template get<WGPUBufferBindingLayout>();
        } else if (j.contains("sampler")) {
            e.sampler = j["sampler"].template get<WGPUSamplerBindingLayout>();
        } else if (j.contains("texture")) {
            e.texture = j["texture"].template get<WGPUTextureBindingLayout>();
        } else if (j.contains("storage_texture")) {
            e.storageTexture = j["storage_texture"].template get<WGPUStorageTextureBindingLayout>();
        } else {
            assert(false);
        }
    }
}

namespace frieren_core {
    WGPUBindGroupLayoutEntry MyWGPUBindGroupLayoutEntry::to_wgpu_entry() const {
        WGPUBindGroupLayoutEntry e;
        e.nextInChain = nullptr;
        e.binding = binding;
        e.visibility = visibility;
        e.buffer = buffer;
        e.sampler = sampler;
        e.texture = texture;
        e.storageTexture = storageTexture;
        return e;
    }

    Shader::~Shader() {
        if (shader_property_buffer != nullptr) {
            wgpuBufferDestroy(shader_property_buffer);
            wgpuBufferRelease(shader_property_buffer);
            shader_property_buffer = nullptr;
        }
        if (bind_group_layout != nullptr) {
            wgpuBindGroupLayoutRelease(bind_group_layout);
            bind_group_layout = nullptr;
        }
        if (render_pipeline != nullptr) {
            wgpuRenderPipelineRelease(render_pipeline);
            render_pipeline = nullptr;
        }
        if (shader_module != nullptr) {
            wgpuShaderModuleRelease(shader_module);
            shader_module = nullptr;
        }
        if (cpu_buffer != nullptr) {
            delete[] cpu_buffer;
            cpu_buffer = nullptr;
        }
    }

    Shader::Shader(WGPUDevice device, const ShaderDescriptor& desc) {
        WGPUShaderModuleDescriptor shader_module_desc;
        shader_module_desc.label = desc.name.c_str();
        WGPUShaderModuleWGSLDescriptor wgsl_desc;
        wgsl_desc.chain.next = nullptr;
        wgsl_desc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
        wgsl_desc.code = desc.shader_source.c_str();
        shader_module_desc.nextInChain = &wgsl_desc.chain;
        shader_property_layout = desc.shader_property_layout_builder.build();

        this->shader_source = desc.shader_source;
        this->shader_module = wgpuDeviceCreateShaderModule(device, &shader_module_desc);
        this->name = desc.name;
        this->instancing = desc.is_instancing;
        this->bind_group_layout_entries = desc.bind_group_layout_entries;

        WGPUBindGroupLayoutDescriptor bind_group_layout_descriptor;
        vector<WGPUBindGroupLayoutEntry> bind_entries;
        for (const auto& item: desc.bind_group_layout_entries) {
            bind_entries.push_back(item.to_wgpu_entry());
        }
        bind_group_layout_descriptor.nextInChain = nullptr;
        bind_group_layout_descriptor.entryCount = bind_entries.size();
        bind_group_layout_descriptor.entries = bind_entries.data();
        bind_group_layout_descriptor.label = (desc.name + "_bindGroupLayout").c_str();

        this->bind_group_layout = wgpuDeviceCreateBindGroupLayout(device, &bind_group_layout_descriptor);

        WGPUPipelineLayoutDescriptor pipeline_layout_descriptor;
        pipeline_layout_descriptor.nextInChain = nullptr;
        pipeline_layout_descriptor.label = (desc.name + "_pipelineLayout").c_str();
        vector<WGPUBindGroupLayout> bind_group_layouts;
        bind_group_layouts.push_back(get_builtin_bind_group_layout(device));
        bind_group_layouts.push_back(bind_group_layout);
        pipeline_layout_descriptor.bindGroupLayoutCount = 1;
        pipeline_layout_descriptor.bindGroupLayouts = bind_group_layouts.data();

        WGPUPipelineLayout pipeline_layout = wgpuDeviceCreatePipelineLayout(device, &pipeline_layout_descriptor);

        WGPURenderPipelineDescriptor render_pipeline_descriptor;

        std::vector<WGPUVertexBufferLayoutOwned> vertex_buffer_layouts_owned;
        vertex_buffer_layouts_owned.push_back(Vertex::get_vertex_buffer_layout());
        std::vector<WGPUVertexBufferLayout> vertex_buffer_layouts;
        for (int i = 0; i < vertex_buffer_layouts_owned.size(); i++) {
            vertex_buffer_layouts.push_back(vertex_buffer_layouts_owned[i].get_wgpu_vertex_buffer_layout());
        }

        render_pipeline_descriptor.nextInChain = nullptr;
        render_pipeline_descriptor.vertex.bufferCount = vertex_buffer_layouts.size();
        // todo instancing buffer layouts
        render_pipeline_descriptor.vertex.buffers = vertex_buffer_layouts.data();
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

        WGPUBufferDescriptor shader_prop_buffer_desc;
        shader_prop_buffer_desc.nextInChain = nullptr;
        shader_prop_buffer_desc.label = (desc.name + "_buffer").c_str();
        shader_prop_buffer_desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
        shader_prop_buffer_desc.size = shader_property_layout.get_total_size();
        shader_prop_buffer_desc.mappedAtCreation = false;

        this->shader_property_buffer = wgpuDeviceCreateBuffer(device, &shader_prop_buffer_desc);

        int shader_property_buffer_size = shader_property_layout.get_total_size();
        this->cpu_buffer = new unsigned char[shader_property_buffer_size];
    }

    WGPUBuffer Shader::get_uniform_buffer() const {
        return shader_property_buffer;
    }

    const vector<MyWGPUBindGroupLayoutEntry>& Shader::get_shader_bind_group_layout_entries_ref() const {
        return bind_group_layout_entries;
    }

    WGPUBindGroupLayout Shader::get_wgpu_bind_group_layout() const {
        return bind_group_layout;
    }

    const ShaderPropertyLayout& Shader::get_shader_property_layout_ref() const {
        return shader_property_layout;
    }

    set<string> Shader::get_texture_names() const {
        set<string> ret;
        for (const auto& t: this->bind_group_layout_entries) {
            if (t.texture.sampleType != WGPUTextureSampleType_Undefined) {
                ret.insert(t.name);
            }
        }
        return std::move(ret);
    }

    set<string> Shader::get_sampler_names() const {
        set<string> ret;
        for (const auto& t: this->bind_group_layout_entries) {
            if (t.sampler.type != WGPUSamplerBindingType_Undefined) {
                ret.insert(t.name);
            }
        }
        return std::move(ret);
    }

    set<string> Shader::get_property_names() const {
        const auto& names = this->shader_property_layout.get_names();
        return {names.begin(), names.end()};
    }
}
