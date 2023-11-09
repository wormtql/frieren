#ifndef FRIEREN_SHADER_H
#define FRIEREN_SHADER_H

#include <common_include.h>
#include <webgpu/webgpu.hpp>
#include "ShaderProperty.h"
#include <nlohmann/json.hpp>
#include "wgpu_serde.h"

using namespace std;
using json = nlohmann::json;

namespace frieren_core {
    struct MyWGPUBindGroupLayoutEntry {
        uint32_t binding;
        WGPUShaderStageFlags visibility;
        string name;

        WGPUBufferBindingLayout buffer;
        WGPUSamplerBindingLayout sampler;
        WGPUTextureBindingLayout texture;
        WGPUStorageTextureBindingLayout storageTexture;

        WGPUBindGroupLayoutEntry to_wgpu_entry() const;
    };

    void from_json(const json& j, MyWGPUBindGroupLayoutEntry& e);

    struct ShaderDescriptor {
        string shader_source_path;
        string shader_source;
        string name;
        string id;

        // triangle or line or ...
        WGPUPrimitiveState primitive_state;
        // enable depth/stencil, compare functions ...
        WGPUDepthStencilState depth_stencil_state;
        // color target format, amount ...
        vector<WGPUColorTargetStateOwned> color_target_states;
        WGPUMultisampleState multisample_state;

        bool is_instancing;

        // bind group layout, including shader properties and shader textures/samplers
        // builtin bind groups not included
        vector<MyWGPUBindGroupLayoutEntry> bind_group_layout_entries;

        // how shader properties(excluding textures/samplers) are layouted
        // it will effect the padding of the uniform buffer
        ShaderPropertyLayoutBuilder shader_property_layout_builder;
    };

    void from_json(const json& j, ShaderDescriptor& shader_desc);

    class Shader {
    private:
        string shader_source;
        WGPUShaderModule shader_module = nullptr;
        string name;
        ShaderPropertyLayout shader_property_layout;
        WGPUBindGroupLayout bind_group_layout;
        // shader property buffer
        WGPUBuffer shader_property_buffer;
        vector<MyWGPUBindGroupLayoutEntry> bind_group_layout_entries;

        bool instancing;

        WGPURenderPipeline render_pipeline;
    public:
        // explicit Shader(WGPUDevice device, const WGPUShaderModuleDescriptor& desc, const ShaderPipelineKey& shader_pipeline_key);
        explicit Shader(WGPUDevice device, const ShaderDescriptor& desc);
        Shader(const Shader& other) = delete;
        ~Shader();

        unsigned char* cpu_buffer = nullptr;

        WGPUBuffer get_uniform_buffer() const;

        const vector<MyWGPUBindGroupLayoutEntry>& get_shader_bind_group_layout_entries_ref() const;

        // get bind group layout of shader-specific properties (shader prop, texture, sampler...)
        WGPUBindGroupLayout get_wgpu_bind_group_layout() const;

        const ShaderPropertyLayout& get_shader_property_layout_ref() const;

        set<string> get_texture_names() const;

        set<string> get_sampler_names() const;

        set<string> get_property_names() const;

        void set_pipeline_for_render_pass(WGPURenderPassEncoder render_pass) const;
    };
}



#endif //FRIEREN_SHADER_H
