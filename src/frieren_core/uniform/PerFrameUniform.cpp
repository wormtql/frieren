#include "PerFrameUniform.h"

namespace frieren_core {
    PerFrameUniform::~PerFrameUniform() {
        if (buffer != nullptr) {
            wgpuBufferDestroy(buffer);
            wgpuBufferRelease(buffer);
            buffer = nullptr;
        }
    }

    void PerFrameUniform::create_wgpu_buffer(WGPUDevice device) {
        WGPUBufferDescriptor desc{};
        desc.nextInChain = nullptr;
        desc.label = "per_frame_uniform";
        desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
        desc.size = sizeof(PerFrameUniformData);
        desc.mappedAtCreation = false;

        buffer = wgpuDeviceCreateBuffer(device, &desc);
    }

    void PerFrameUniform::update_uniform_buffer(WGPUQueue queue) {
        auto size = sizeof(PerFrameUniformData);

        wgpuQueueWriteBuffer(queue, buffer, 0, &data, size);
    }
}
