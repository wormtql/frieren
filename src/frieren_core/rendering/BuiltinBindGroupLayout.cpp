#include "BuiltinBindGroupLayout.h"
#include <utilities/BindGroupLayoutBuilder.h>
#include <utilities/BindGroupBuilder.h>

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

    BuiltinBindGroup::BuiltinBindGroup() {
    }

    BuiltinBindGroup::~BuiltinBindGroup() {
        if (this->bind_group != nullptr) {
            wgpuBindGroupRelease(bind_group);
            bind_group = nullptr;
        }
        if (this->bind_group_layout != nullptr) {
            wgpuBindGroupLayoutRelease(bind_group_layout);
            bind_group_layout = nullptr;
        }
    }

    void BuiltinBindGroup::init(WGPUDevice device) {
        this->bind_group_layout = get_builtin_bind_group_layout(device);

        this->per_frame_uniform.create_wgpu_buffer(device);
        this->per_object_uniform.create_wgpu_buffer(device);

        BindGroupBuilder builder;
        builder.add_whole_buffer(0, per_frame_uniform.get_buffer());
        builder.add_whole_buffer(1, per_object_uniform.get_buffer());
        this->bind_group = builder.build(device, bind_group_layout);
    }
}