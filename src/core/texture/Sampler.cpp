#include "Sampler.h"

namespace frieren_core {
    Sampler::Sampler(WGPUDevice device, const WGPUSamplerDescriptor& desc) {
        this->sampler = wgpuDeviceCreateSampler(device, &desc);
    }

    Sampler::~Sampler() {
        if (this->sampler) {
            wgpuSamplerRelease(sampler);
            sampler = nullptr;
        }
    }

    WGPUSampler Sampler::get_wgpu_sampler() {
        return sampler;
    }
}