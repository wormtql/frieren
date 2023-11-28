#include "Sampler.h"
#include <wgpu_serde.h>

namespace frieren_core {
    void from_json(const json& j, SamplerDescriptor& desc) {
        desc.name = j["name"];
        desc.id = j["id"];
        desc.addressModeU = j["address_mode_u"].template get<WGPUAddressMode>();
        desc.addressModeV = j["address_mode_v"].template get<WGPUAddressMode>();
        desc.addressModeW = j["address_mode_w"].template get<WGPUAddressMode>();
        desc.magFilter = j["mag_filter"].template get<WGPUFilterMode>();
        desc.minFilter = j["min_filter"].template get<WGPUFilterMode>();
        desc.mipmapFilter = j["mipmap_filter"].template get<WGPUMipmapFilterMode>();
        desc.lodMinClamp = j["lod_min_clamp"];
        desc.lodMaxClamp = j["lod_max_clamp"];
        desc.compare = j["compare"].template get<WGPUCompareFunction>();
        desc.maxAnisotropy = j["max_anisotropy"];
    }

    WGPUSamplerDescriptor SamplerDescriptor::get_wgpu_sampler_descriptor() const {
        WGPUSamplerDescriptor desc{};

        desc.nextInChain = nullptr;
        desc.label = name.c_str();
        desc.addressModeU = addressModeU;
        desc.addressModeV = addressModeV;
        desc.addressModeW = addressModeW;
        desc.magFilter = magFilter;
        desc.minFilter = minFilter;
        desc.mipmapFilter = mipmapFilter;
        desc.lodMinClamp = lodMinClamp;
        desc.lodMaxClamp = lodMaxClamp;
        desc.compare = compare;
        desc.maxAnisotropy = maxAnisotropy;

        return desc;
    }

    Sampler::Sampler(WGPUDevice device, const SamplerDescriptor& desc) {
        WGPUSamplerDescriptor wgpu_desc = desc.get_wgpu_sampler_descriptor();

        this->sampler = wgpuDeviceCreateSampler(device, &wgpu_desc);
    }

    Sampler::~Sampler() {
        if (this->sampler) {
            wgpuSamplerRelease(sampler);
            sampler = nullptr;
        }
    }

    WGPUSampler Sampler::get_wgpu_sampler() const {
        return sampler;
    }
}