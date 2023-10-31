#include "Uniform.h"

namespace frieren_core {
    SimpleTypeUniform::SimpleTypeUniform(WGPUDevice device, const ShaderPropertyLayoutBuilder layout_builder, const std::string& label) {
        this->uniform_layout = layout_builder.build();
        this->size = uniform_layout.get_total_size();
        this->label = label;

        WGPUBufferDescriptor buffer_desc;
        buffer_desc.nextInChain = nullptr;
        buffer_desc.label = label.c_str();
        buffer_desc.size = uniform_layout.get_total_size();
        buffer_desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;

        this->uniform_buffer = wgpuDeviceCreateBuffer(device, &buffer_desc);
    }

    SimpleTypeUniform::~SimpleTypeUniform() {
        if (uniform_buffer != nullptr) {
            wgpuBufferRelease(uniform_buffer);
            wgpuBufferDestroy(uniform_buffer);
            uniform_buffer = nullptr;
        }
    }
}