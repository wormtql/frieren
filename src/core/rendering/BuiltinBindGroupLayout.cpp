#include "BuiltinBindGroupLayout.h"
#include <utilities/BindGroupLayoutBuilder.h>

using std::vector;

namespace frieren_core {
    WGPUBindGroupLayout get_builtin_bind_group_layout(WGPUDevice device) {
        BindGroupLayoutBuilder builder{};
        // transformation
        builder.add_uniform_buffer(0);
        // camera
        builder.add_uniform_buffer(1);
        // todo add more bindings

        return builder.build(device);
    }
}