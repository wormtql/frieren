#include "PerObjectUniform.h"

namespace frieren_core {
    PerObjectUniform::~PerObjectUniform() {
        if (buffer != nullptr) {
            wgpuBufferDestroy(buffer);
            wgpuBufferRelease(buffer);
            buffer = nullptr;
        }
    }

    void PerObjectUniform::create_wgpu_buffer(WGPUDevice device) {
        WGPUBufferDescriptor desc{};
        desc.nextInChain = nullptr;
        desc.label = "per_object_uniform";
        desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
        desc.size = sizeof(PerObjectUniformData);
        desc.mappedAtCreation = false;

        buffer = wgpuDeviceCreateBuffer(device, &desc);
    }

    void PerObjectUniform::update_uniform_buffer(WGPUQueue queue) {
        wgpuQueueWriteBuffer(queue, buffer, 0, &data, sizeof(PerObjectUniformData));
    }
}
