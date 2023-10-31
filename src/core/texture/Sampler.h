#ifndef FRIEREN_CORE_SAMPLER_H
#define FRIEREN_CORE_SAMPLER_H

#include <webgpu/webgpu.hpp>

namespace frieren_core {
    class Sampler {
    private:
        WGPUSampler sampler;
    public:
        explicit Sampler(WGPUDevice device, const WGPUSamplerDescriptor& desc);
        ~Sampler();

        WGPUSampler get_wgpu_sampler() const;
    };
}

#endif
