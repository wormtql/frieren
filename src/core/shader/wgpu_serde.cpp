#include "wgpu_serde.h"
#include <common_include.h>

using namespace std;

void from_json(const json& j, WGPUBlendState& blend_state) {
    if (j.contains("color")) {
        blend_state.color = j["color"].template get<WGPUBlendComponent>();
    }
    if (j.contains("alpha")) {
        blend_state.alpha = j["alpha"].template get<WGPUBlendComponent>();
    }
}

void from_json(const json& j, WGPUBlendComponent& blend_component) {
    auto operation = j["operation"].template get<WGPUBlendOperation>();
    auto src_factor = j["src_factor"].template get<WGPUBlendFactor>();
    auto dst_factor = j["dst_factor"].template get<WGPUBlendFactor>();

    blend_component.operation = operation;
    blend_component.srcFactor = src_factor;
    blend_component.dstFactor = dst_factor;
}

void from_json(const json& j, WGPUBlendOperation& blend_operation) {
    string s = j.template get<string>();
    if (s == "Add") {
        blend_operation = WGPUBlendOperation_Add;
    } else if (s == "Subtract") {
        blend_operation = WGPUBlendOperation_Subtract;
    } else if (s == "ReverseSubtract") {
        blend_operation = WGPUBlendOperation_ReverseSubtract;
    } else if (s == "Min") {
        blend_operation = WGPUBlendOperation_Min;
    } else if (s == "Max") {
        blend_operation = WGPUBlendOperation_Max;
    } else {
        assert(false);
    }
}

void from_json(const json& j, WGPUBlendFactor& blend_factor) {
    string s = j.template get<string>();
    if (s == "Zero") {
        blend_factor = WGPUBlendFactor_Zero;
    } else if (s == "One") {
        blend_factor = WGPUBlendFactor_One;
    } else if (s == "Src") {
        blend_factor = WGPUBlendFactor_Src;
    } else if (s == "OneMinusSrc") {
        blend_factor = WGPUBlendFactor_OneMinusSrc;
    } else if (s == "SrcAlpha") {
        blend_factor = WGPUBlendFactor_SrcAlpha;
    } else if (s == "OneMinusSrcAlpha") {
        blend_factor = WGPUBlendFactor_OneMinusSrcAlpha;
    } else if (s == "Dst") {
        blend_factor = WGPUBlendFactor_Dst;
    } else if (s == "OneMinusDst") {
        blend_factor = WGPUBlendFactor_OneMinusDst;
    } else if (s == "DstAlpha") {
        blend_factor = WGPUBlendFactor_DstAlpha;
    } else if (s == "OneMinusDstAlpha") {
        blend_factor = WGPUBlendFactor_OneMinusDstAlpha;
    } else if (s == "SrcAlphaSaturated") {
        blend_factor = WGPUBlendFactor_SrcAlphaSaturated;
    } else if (s == "Constant") {
        blend_factor = WGPUBlendFactor_Constant;
    } else if (s == "OneMinusConstant") {
        blend_factor = WGPUBlendFactor_OneMinusConstant;
    } else {
        assert(false);
    }
}

void from_json(const json& j, WGPUCullMode& cull_mode) {
    string s = j.template get<string>();
    if (s == "None") {
        cull_mode = WGPUCullMode_None;
    } else if (s == "Front") {
        cull_mode = WGPUCullMode_Front;
    } else if (s == "Back") {
        cull_mode = WGPUCullMode_Back;
    } else {
        assert(false);
    }
}

void from_json(const json& j, WGPUBindGroupLayoutEntry& e) {
    e.nextInChain = nullptr;
    e.binding = j["binding"];
    e.visibility = WGPUShaderStage_Fragment | WGPUShaderStage_Fragment;

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

void from_json(const json& j, WGPUBufferBindingLayout& buffer_binding_layout) {
    buffer_binding_layout.nextInChain = nullptr;
    buffer_binding_layout.type = j["type"].template get<WGPUBufferBindingType>();
    buffer_binding_layout.hasDynamicOffset = j["has_dynamic_offset"];
    buffer_binding_layout.minBindingSize = 0;
}

void from_json(const json& j, WGPUBufferBindingType& buffer_binding_type) {
    string s = j.template get<string>();
    if (s == "Uniform") {
        buffer_binding_type = WGPUBufferBindingType_Uniform;
    } else if (s == "Storage") {
        buffer_binding_type = WGPUBufferBindingType_Storage;
    } else if (s == "ReadOnlyStorage") {
        buffer_binding_type = WGPUBufferBindingType_ReadOnlyStorage;
    } else {
        buffer_binding_type = WGPUBufferBindingType_Undefined;
    }
}

void from_json(const json& j, WGPUSamplerBindingType& sampler_binding_type) {
    string s = j.template get<string>();
    if (s == "Filtering") {
        sampler_binding_type = WGPUSamplerBindingType_Filtering;
    } else if (s == "NonFiltering") {
        sampler_binding_type = WGPUSamplerBindingType_NonFiltering;
    } else if (s == "Comparison") {
        sampler_binding_type = WGPUSamplerBindingType_Comparison;
    } else {
        sampler_binding_type = WGPUSamplerBindingType_Undefined;
    }
}

void from_json(const json& j, WGPUSamplerBindingLayout& sampler_binding_layout) {
    sampler_binding_layout.nextInChain = nullptr;
    sampler_binding_layout.type = j["type"].template get<WGPUSamplerBindingType>();
}

void from_json(const json& j, WGPUTextureBindingLayout& texture_binding_layout) {
    texture_binding_layout.nextInChain = nullptr;
    texture_binding_layout.multisampled = j["multisampled"];
    texture_binding_layout.sampleType = j["sample_type"].template get<WGPUTextureSampleType>();
    texture_binding_layout.viewDimension = j["view_dimension"].template get<WGPUTextureViewDimension>();
}

void from_json(const json& j, WGPUTextureSampleType& texture_sample_type) {
    string s = j.template get<string>();
    if (s == "Float") {
        texture_sample_type = WGPUTextureSampleType_Float;
    } else if (s == "UnfilterableFloat") {
        texture_sample_type = WGPUTextureSampleType_UnfilterableFloat;
    } else if (s == "Depth") {
        texture_sample_type = WGPUTextureSampleType_Depth;
    } else if (s == "Sint") {
        texture_sample_type = WGPUTextureSampleType_Sint;
    } else if (s == "Uint") {
        texture_sample_type = WGPUTextureSampleType_Uint;
    } else {
        texture_sample_type = WGPUTextureSampleType_Undefined;
    }
}

void from_json(const json& j, WGPUTextureViewDimension& texture_view_dimension) {
    string s = j.template get<string>();
    if (s == "1D") {
        texture_view_dimension = WGPUTextureViewDimension_1D;
    } else if (s == "2D") {
        texture_view_dimension = WGPUTextureViewDimension_2D;
    } else if (s == "2DArray") {
        texture_view_dimension = WGPUTextureViewDimension_2DArray;
    } else if (s == "Cube") {
        texture_view_dimension = WGPUTextureViewDimension_Cube;
    } else if (s == "CubeArray") {
        texture_view_dimension = WGPUTextureViewDimension_CubeArray;
    } else if (s == "3D") {
        texture_view_dimension = WGPUTextureViewDimension_3D;
    } else {
        texture_view_dimension = WGPUTextureViewDimension_Undefined;
    }
}

void from_json(const json& j, WGPUStorageTextureAccess& access) {
    string s = j.template get<string>();
    if (s == "WriteOnly") {
        access = WGPUStorageTextureAccess_WriteOnly;
    } else {
        access = WGPUStorageTextureAccess_Undefined;
    }
}

void from_json(const json& j, WGPUStorageTextureBindingLayout& layout) {
    layout.nextInChain = nullptr;
    layout.access = j["access"].template get<WGPUStorageTextureAccess>();
    layout.format = j["format"].template get<WGPUTextureFormat>();
    layout.viewDimension = j["view_dimension"].template get<WGPUTextureViewDimension>();
}

void from_json(const json& j, WGPUDepthStencilState& depth_stencil_state) {
    depth_stencil_state.nextInChain = nullptr;
    if (j.contains("format")) {
        depth_stencil_state.format = j["format"].template get<WGPUTextureFormat>();
    } else {
        depth_stencil_state.format = WGPUTextureFormat_Undefined;
    }
    depth_stencil_state.depthWriteEnabled = j["depth_write_enabled"];
    depth_stencil_state.depthCompare = j["depth_compare"].template get<WGPUCompareFunction>();
    depth_stencil_state.stencilFront = j["stencil_front"].template get<WGPUStencilFaceState>();
    depth_stencil_state.stencilBack = j["stencil_back"].template get<WGPUStencilFaceState>();
    depth_stencil_state.stencilReadMask = j["stencil_read_mask"];
    depth_stencil_state.stencilWriteMask = j["stencil_write_mask"];
    depth_stencil_state.depthBias = j["depth_bias"];
    depth_stencil_state.depthBiasSlopeScale = j["depth_bias_slope_scale"];
    depth_stencil_state.depthBiasClamp = j["depth_bias_clamp"];
}

void from_json(const json& j, WGPUStencilFaceState& stencil_face_state) {
    stencil_face_state.compare = j["compare"].template get<WGPUCompareFunction>();
    stencil_face_state.depthFailOp = j["depth_fail_op"].template get<WGPUStencilOperation>();
    stencil_face_state.failOp = j["fail_op"].template get<WGPUStencilOperation>();
    stencil_face_state.passOp = j["pass_op"].template get<WGPUStencilOperation>();
}

void from_json(const json& j, WGPUPrimitiveState& primitive_state) {
    primitive_state.nextInChain = nullptr;
    primitive_state.topology = j["topology"].template get<WGPUPrimitiveTopology>();
    primitive_state.stripIndexFormat = j["stripIndexFormat"].template get<WGPUIndexFormat>();
    primitive_state.frontFace = j["front_face"].template get<WGPUFrontFace>();
    primitive_state.cullMode = j["cull_mode"].template get<WGPUCullMode>();
}

void from_json(const json& j, WGPUColorWriteMask& flags) {
    auto get_flag = [] (const json& jj) {
        string s = jj.template get<string>();
        if (s == "None") {
            return WGPUColorWriteMask_None;
        } else if (s == "Red") {
            return WGPUColorWriteMask_Red;
        } else if (s == "Green") {
            return WGPUColorWriteMask_Green;
        } else if (s == "Blue") {
            return WGPUColorWriteMask_Blue;
        } else if (s == "Alpha") {
            return WGPUColorWriteMask_Alpha;
        } else if (s == "All") {
            return WGPUColorWriteMask_All;
        } else {
            assert(false);
        }
        return WGPUColorWriteMask_All;
    };

    if (j.is_string()) {
        flags = get_flag(j);
    } else if (j.is_array()) {
        flags = WGPUColorWriteMask_None;
        for (auto it = j.begin(); it != j.end(); it++) {
            flags = static_cast<WGPUColorWriteMask>(flags | get_flag(*it));
        }
    } else {
        assert(false);
    }

    flags = WGPUColorWriteMask_All;
}

WGPUColorTargetState WGPUColorTargetStateOwned::get_wgpu_state() const {
    WGPUColorTargetState ret;
    ret.nextInChain = this->nextInChain;
    ret.format = this->format;
    ret.blend = &this->blend;
    ret.writeMask = this->writeMask;
    return ret;
}

void from_json(const json& j, WGPUColorTargetStateOwned& state) {
    state.nextInChain = nullptr;
    state.format = j["format"].template get<WGPUTextureFormat>();
    state.blend = j["blend"].template get<WGPUBlendState>();
    state.writeMask = j["write_mask"].template get<WGPUColorWriteMask>();
}

void from_json(const json& j, WGPUMultisampleState& state) {
    state.nextInChain = nullptr;
    state.count = j["count"];
    state.mask = j["mask"];
    state.alphaToCoverageEnabled = j["alpha_to_coverage_enabled"];
}
