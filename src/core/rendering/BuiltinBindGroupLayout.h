#ifndef FRIEREN_CORE_BIND_GROUP_LAYOUT_H
#define FRIEREN_CORE_BIND_GROUP_LAYOUT_H

#include <webgpu/webgpu.hpp>
#include <common_include_glm.h>

namespace frieren_core {
    // camera, lighting, transformation, time ...
    WGPUBindGroupLayout get_builtin_bind_group_layout(WGPUDevice device);
}

#endif
