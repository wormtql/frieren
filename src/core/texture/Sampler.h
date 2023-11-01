#ifndef FRIEREN_CORE_SAMPLER_H
#define FRIEREN_CORE_SAMPLER_H

#include <webgpu/webgpu.hpp>
#include <common_include.h>

using namespace std;

namespace frieren_core {
    struct SamplerDescriptor {
        string name;
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

        WGPUSamplerDescriptor get_wgpu_sampler_descriptor() const;
    };

    void from_json(const json& j, SamplerDescriptor& desc);

    class Sampler {
    private:
        WGPUSampler sampler = nullptr;
    public:
        explicit Sampler(WGPUDevice device, const SamplerDescriptor& desc);
        Sampler(const Sampler& other) = delete;
        ~Sampler();

        WGPUSampler get_wgpu_sampler() const;
    };
}

#endif
