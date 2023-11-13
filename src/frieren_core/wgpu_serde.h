#ifndef FRIEREN_CORE_WGPU_SERDE_H
#define FRIEREN_CORE_WGPU_SERDE_H

#define JSON_DISABLE_ENUM_SERIALIZATION 1

#include <nlohmann/json.hpp>
#include <webgpu/webgpu.hpp>

using json = nlohmann::json;
using std::string;

void from_json(const json& j, WGPUBlendOperation& blend_operation);

void from_json(const json& j, WGPUBlendState& blend_state);

void from_json(const json& j, WGPUBlendComponent& blend_component);

void from_json(const json& j, WGPUBlendFactor& blend_factor);

void from_json(const json& j, WGPUCullMode& cull_mode);

void from_json(const json& j, WGPUBindGroupLayoutEntry& e);

void from_json(const json& j, WGPUBufferBindingLayout& buffer_binding_layout);

void from_json(const json& j, WGPUBufferBindingType& buffer_binding_type);

void from_json(const json& j, WGPUSamplerBindingLayout& sampler_binding_layout);

void from_json(const json& j, WGPUSamplerBindingType& sampler_binding_type);

void from_json(const json& j, WGPUTextureBindingLayout& texture_binding_layout);

void from_json(const json& j, WGPUTextureSampleType& texture_sample_type);

void from_json(const json& j, WGPUTextureViewDimension& texture_view_dimension);

void from_json(const json& j, WGPUStorageTextureAccess& access);

void from_json(const json& j, WGPUStorageTextureBindingLayout& layout);

void from_json(const json& j, WGPUDepthStencilState& depth_stencil_state);

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUCompareFunction, {
    {WGPUCompareFunction_Undefined, "Undefined"},
    {WGPUCompareFunction_Never, "Never"},
    {WGPUCompareFunction_Less, "Less"},
    {WGPUCompareFunction_LessEqual, "LessEqual"},
    {WGPUCompareFunction_Greater, "Greater"},
    {WGPUCompareFunction_GreaterEqual, "GreaterEqual"},
    {WGPUCompareFunction_Equal, "Equal"},
    {WGPUCompareFunction_NotEqual, "NotEqual"},
    {WGPUCompareFunction_Always, "Always"},
    {WGPUCompareFunction_Force32, "Force32"}
})

void from_json(const json& j, WGPUStencilFaceState& stencil_face_state);

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUStencilOperation, {
    {WGPUStencilOperation_Keep, "Keep"},
    {WGPUStencilOperation_Zero, "Zero"},
    {WGPUStencilOperation_Replace, "Replace"},
    {WGPUStencilOperation_Invert, "Invert"},
    {WGPUStencilOperation_IncrementClamp, "IncrementClamp"},
    {WGPUStencilOperation_DecrementClamp, "DecrementClamp"},
    {WGPUStencilOperation_IncrementWrap, "IncrementWrap"},
    {WGPUStencilOperation_DecrementWrap, "DecrementWrap"},
    {WGPUStencilOperation_Force32, "Force32"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUPrimitiveTopology, {
    {WGPUPrimitiveTopology_PointList, "PointList"},
    {WGPUPrimitiveTopology_LineList, "LineList"},
    {WGPUPrimitiveTopology_LineStrip, "LineStrip"},
    {WGPUPrimitiveTopology_TriangleList, "TriangleList"},
    {WGPUPrimitiveTopology_TriangleStrip, "TriangleStrip"},
    {WGPUPrimitiveTopology_Force32, "Force32"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUIndexFormat, {
    {WGPUIndexFormat_Undefined, nullptr},
    {WGPUIndexFormat_Uint16, "Uint16"},
    {WGPUIndexFormat_Uint32, "Uint32"},
    {WGPUIndexFormat_Force32, "Force32"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUFrontFace, {
    {WGPUFrontFace_CCW, "CCW"},
    {WGPUFrontFace_CW, "CW"},
    {WGPUFrontFace_Force32, "Force32"}
})

void from_json(const json& j, WGPUPrimitiveState& primitive_state);

void from_json(const json& j, WGPUColorWriteMask& flags);

struct WGPUColorTargetStateOwned {
    WGPUChainedStruct const * nextInChain;
    WGPUTextureFormat format;
    WGPUBlendState blend;
    WGPUColorWriteMaskFlags writeMask;

    WGPUColorTargetState get_wgpu_state() const;
};

void from_json(const json& j, WGPUColorTargetStateOwned& state);

void from_json(const json& j, WGPUMultisampleState& state);

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUAddressMode, {
    {WGPUAddressMode_ClampToEdge, "ClampToEdge"},
    {WGPUAddressMode_Repeat, "Repeat"},
    {WGPUAddressMode_MirrorRepeat, "MirrorRepeat"}
})

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUFilterMode, {
    {WGPUFilterMode_Linear, "Linear"},
    {WGPUFilterMode_Nearest, "Nearest"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUMipmapFilterMode, {
    {WGPUMipmapFilterMode_Linear, "Linear"},
    {WGPUMipmapFilterMode_Nearest, "Nearest"}
})

struct WGPUSamplerDescriptorOwned {
    string label;
    WGPUAddressMode addressModeU;
    WGPUAddressMode addressModeV;
    WGPUAddressMode addressModeW;
    WGPUFilterMode magFilter;
    WGPUFilterMode minFilter;
    WGPUMipmapFilterMode mipmapFilter;
    float lodMinClamp;
    float lodMaxClamp;
    WGPUCompareFunction compare;
    uint16_t maxAnisotropy;
};

void from_json(const json& j, WGPUSamplerDescriptorOwned& desc);

void from_json(const json& j, WGPUShaderStageFlags& flags);

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUTextureDimension, {
    {WGPUTextureDimension_1D, "1D"},
    {WGPUTextureDimension_2D, "2D"},
    {WGPUTextureDimension_3D, "3D"},
})

NLOHMANN_JSON_SERIALIZE_ENUM(WGPUTextureFormat, {
    {WGPUTextureFormat_Undefined, "Undefined"},
    {WGPUTextureFormat_R8Unorm, "R8Unorm"},
    {WGPUTextureFormat_R8Snorm, "R8Snorm"},
    {WGPUTextureFormat_R8Uint, "R8Uint"},
    {WGPUTextureFormat_R8Sint, "R8Sint"},
    {WGPUTextureFormat_R16Uint, "R16Uint"},
    {WGPUTextureFormat_R16Sint, "R16Sint"},
    {WGPUTextureFormat_R16Float, "R16Float"},
    {WGPUTextureFormat_RG8Unorm, "RG8Unorm"},
    {WGPUTextureFormat_RG8Snorm, "RG8Snorm"},
    {WGPUTextureFormat_RG8Uint, "RG8Uint"},
    {WGPUTextureFormat_RG8Sint, "RG8Sint"},
    {WGPUTextureFormat_R32Float, "R32Float"},
    {WGPUTextureFormat_R32Uint, "R32Uint"},
    {WGPUTextureFormat_R32Sint, "R32Sint"},
    {WGPUTextureFormat_RG16Uint, "RG16Uint"},
    {WGPUTextureFormat_RG16Sint, "RG16Sint"},
    {WGPUTextureFormat_RG16Float, "RG16Float"},
    {WGPUTextureFormat_RGBA8Unorm, "RGBA8Unorm"},
    {WGPUTextureFormat_RGBA8UnormSrgb, "RGBA8UnormSrgb"},
    {WGPUTextureFormat_RGBA8Snorm, "RGBA8Snorm"},
    {WGPUTextureFormat_RGBA8Uint, "RGBA8Uint"},
    {WGPUTextureFormat_RGBA8Sint, "RGBA8Sint"},
    {WGPUTextureFormat_BGRA8Unorm, "BGRA8Unorm"},
    {WGPUTextureFormat_BGRA8UnormSrgb, "BGRA8UnormSrgb"},
    {WGPUTextureFormat_RGB10A2Unorm, "RGB10A2Unorm"},
    {WGPUTextureFormat_RG11B10Ufloat, "RG11B10Ufloat"},
    {WGPUTextureFormat_RGB9E5Ufloat, "RGB9E5Ufloat"},
    {WGPUTextureFormat_RG32Float, "RG32Float"},
    {WGPUTextureFormat_RG32Uint, "RG32Uint"},
    {WGPUTextureFormat_RG32Sint, "RG32Sint"},
    {WGPUTextureFormat_RGBA16Uint, "RGBA16Uint"},
    {WGPUTextureFormat_RGBA16Sint, "RGBA16Sint"},
    {WGPUTextureFormat_RGBA16Float, "RGBA16Float"},
    {WGPUTextureFormat_RGBA32Float, "RGBA32Float"},
    {WGPUTextureFormat_RGBA32Uint, "RGBA32Uint"},
    {WGPUTextureFormat_RGBA32Sint, "RGBA32Sint"},
    {WGPUTextureFormat_Stencil8, "Stencil8"},
    {WGPUTextureFormat_Depth16Unorm, "Depth16Unorm"},
    {WGPUTextureFormat_Depth24Plus, "Depth24Plus"},
    {WGPUTextureFormat_Depth24PlusStencil8, "Depth24PlusStencil8"},
    {WGPUTextureFormat_Depth32Float, "Depth32Float"},
    {WGPUTextureFormat_Depth32FloatStencil8, "Depth32FloatStencil8"},
    {WGPUTextureFormat_BC1RGBAUnorm, "BC1RGBAUnorm"},
    {WGPUTextureFormat_BC1RGBAUnormSrgb, "BC1RGBAUnormSrgb"},
    {WGPUTextureFormat_BC2RGBAUnorm, "BC2RGBAUnorm"},
    {WGPUTextureFormat_BC2RGBAUnormSrgb, "BC2RGBAUnormSrgb"},
    {WGPUTextureFormat_BC3RGBAUnorm, "BC3RGBAUnorm"},
    {WGPUTextureFormat_BC3RGBAUnormSrgb, "BC3RGBAUnormSrgb"},
    {WGPUTextureFormat_BC4RUnorm, "BC4RUnorm"},
    {WGPUTextureFormat_BC4RSnorm, "BC4RSnorm"},
    {WGPUTextureFormat_BC5RGUnorm, "BC5RGUnorm"},
    {WGPUTextureFormat_BC5RGSnorm, "BC5RGSnorm"},
    {WGPUTextureFormat_BC6HRGBUfloat, "BC6HRGBUfloat"},
    {WGPUTextureFormat_BC6HRGBFloat, "BC6HRGBFloat"},
    {WGPUTextureFormat_BC7RGBAUnorm, "BC7RGBAUnorm"},
    {WGPUTextureFormat_BC7RGBAUnormSrgb, "BC7RGBAUnormSrgb"},
    {WGPUTextureFormat_ETC2RGB8Unorm, "ETC2RGB8Unorm"},
    {WGPUTextureFormat_ETC2RGB8UnormSrgb, "ETC2RGB8UnormSrgb"},
    {WGPUTextureFormat_ETC2RGB8A1Unorm, "ETC2RGB8A1Unorm"},
    {WGPUTextureFormat_ETC2RGB8A1UnormSrgb, "ETC2RGB8A1UnormSrgb"},
    {WGPUTextureFormat_ETC2RGBA8Unorm, "ETC2RGBA8Unorm"},
    {WGPUTextureFormat_ETC2RGBA8UnormSrgb, "ETC2RGBA8UnormSrgb"},
    {WGPUTextureFormat_EACR11Unorm, "EACR11Unorm"},
    {WGPUTextureFormat_EACR11Snorm, "EACR11Snorm"},
    {WGPUTextureFormat_EACRG11Unorm, "EACRG11Unorm"},
    {WGPUTextureFormat_EACRG11Snorm, "EACRG11Snorm"},
    {WGPUTextureFormat_ASTC4x4Unorm, "ASTC4x4Unorm"},
    {WGPUTextureFormat_ASTC4x4UnormSrgb, "ASTC4x4UnormSrgb"},
    {WGPUTextureFormat_ASTC5x4Unorm, "ASTC5x4Unorm"},
    {WGPUTextureFormat_ASTC5x4UnormSrgb, "ASTC5x4UnormSrgb"},
    {WGPUTextureFormat_ASTC5x5Unorm, "ASTC5x5Unorm"},
    {WGPUTextureFormat_ASTC5x5UnormSrgb, "ASTC5x5UnormSrgb"},
    {WGPUTextureFormat_ASTC6x5Unorm, "ASTC6x5Unorm"},
    {WGPUTextureFormat_ASTC6x5UnormSrgb, "ASTC6x5UnormSrgb"},
    {WGPUTextureFormat_ASTC6x6Unorm, "ASTC6x6Unorm"},
    {WGPUTextureFormat_ASTC6x6UnormSrgb, "ASTC6x6UnormSrgb"},
    {WGPUTextureFormat_ASTC8x5Unorm, "ASTC8x5Unorm"},
    {WGPUTextureFormat_ASTC8x5UnormSrgb, "ASTC8x5UnormSrgb"},
    {WGPUTextureFormat_ASTC8x6Unorm, "ASTC8x6Unorm"},
    {WGPUTextureFormat_ASTC8x6UnormSrgb, "ASTC8x6UnormSrgb"},
    {WGPUTextureFormat_ASTC8x8Unorm, "ASTC8x8Unorm"},
    {WGPUTextureFormat_ASTC8x8UnormSrgb, "ASTC8x8UnormSrgb"},
    {WGPUTextureFormat_ASTC10x5Unorm, "ASTC10x5Unorm"},
    {WGPUTextureFormat_ASTC10x5UnormSrgb, "ASTC10x5UnormSrgb"},
    {WGPUTextureFormat_ASTC10x6Unorm, "ASTC10x6Unorm"},
    {WGPUTextureFormat_ASTC10x6UnormSrgb, "ASTC10x6UnormSrgb"},
    {WGPUTextureFormat_ASTC10x8Unorm, "ASTC10x8Unorm"},
    {WGPUTextureFormat_ASTC10x8UnormSrgb, "ASTC10x8UnormSrgb"},
    {WGPUTextureFormat_ASTC10x10Unorm, "ASTC10x10Unorm"},
    {WGPUTextureFormat_ASTC10x10UnormSrgb, "ASTC10x10UnormSrgb"},
    {WGPUTextureFormat_ASTC12x10Unorm, "ASTC12x10Unorm"},
    {WGPUTextureFormat_ASTC12x10UnormSrgb, "ASTC12x10UnormSrgb"},
    {WGPUTextureFormat_ASTC12x12Unorm, "ASTC12x12Unorm"},
    {WGPUTextureFormat_ASTC12x12UnormSrgb, "ASTC12x12UnormSrgb"},
    {WGPUTextureFormat_Force32, "Force32"}
})

#endif
