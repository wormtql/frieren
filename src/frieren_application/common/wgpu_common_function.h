#ifndef FRIEREN_APPLICATION_WGPU_COMMON_FUNCTION_H
#define FRIEREN_APPLICATION_WGPU_COMMON_FUNCTION_H

#include <webgpu/webgpu.hpp>

namespace frieren_application {
    class WGPUCommonFunction {
    public:
        static void inspect_adapter(WGPUAdapter adapter);

        static WGPUDevice request_device(const WGPUDeviceDescriptor& desc, WGPUAdapter adapter);

        static WGPUAdapter request_adapter(const WGPURequestAdapterOptions& options, WGPUInstance instance);

        static void inspect_device(WGPUDevice device);
    };
}

#endif
