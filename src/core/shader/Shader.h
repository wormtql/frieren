//
// Created by 58413 on 2023/10/16.
//

#ifndef FRIEREN_SHADER_H
#define FRIEREN_SHADER_H

#include <common_include.h>
#include <webgpu/webgpu.hpp>
#include "ShaderPipelineKey.h"
#include <nlohmann/json.hpp>
#include "wgpu_serde.h"

using namespace std;
using json = nlohmann::json;

namespace frieren_core {
    struct ShaderDescriptor {
        string shader_source_path;
        string shader_source;
        string name;
        WGPUPrimitiveState primitive_state;
        WGPUDepthStencilState depth_stencil_state;
        vector<WGPUColorTargetStateOwned> color_target_states;
        WGPUMultisampleState multisample_state;
        bool is_instancing;
        vector<WGPUBindGroupLayoutEntry> bind_group_layout_entries;
    };

    void from_json(const json& j, ShaderDescriptor& shader_desc);

    // void to_json(json& j, const ShaderDescriptor& shader_desc) {
    //     j = json{
    //         {""}
    //     }
    // }

    class Shader {
    private:
        string shader_source;
        WGPUShaderModule shader_module = nullptr;
        string name;

        bool instancing;

        WGPURenderPipeline render_pipeline;
    public:
        // explicit Shader(WGPUDevice device, const WGPUShaderModuleDescriptor& desc, const ShaderPipelineKey& shader_pipeline_key);
        explicit Shader(WGPUDevice device, const ShaderDescriptor& desc);
    };
}



#endif //FRIEREN_SHADER_H
