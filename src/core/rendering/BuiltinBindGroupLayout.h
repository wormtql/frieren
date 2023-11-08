#ifndef FRIEREN_CORE_BIND_GROUP_LAYOUT_H
#define FRIEREN_CORE_BIND_GROUP_LAYOUT_H

#include <webgpu/webgpu.hpp>
#include <common_include_glm.h>
#include <uniform/PerFrameUniform.h>
#include <uniform/PerObjectUniform.h>

namespace frieren_core {
    // camera, lighting, transformation, time ...
    WGPUBindGroupLayout get_builtin_bind_group_layout(WGPUDevice device);

    class BuiltinBindGroup {
    public:
        WGPUBindGroup bind_group = nullptr;
        WGPUBindGroupLayout bind_group_layout = nullptr;

        PerFrameUniform per_frame_uniform;
        PerObjectUniform per_object_uniform;

        BuiltinBindGroup();
        ~BuiltinBindGroup();

        /**
         * @brief init builtin bind group related data
         * 1. create bind group layout
         * 2. create buffers
         * 3. create bind group
         */
        void init(WGPUDevice device);
    };
}

#endif
