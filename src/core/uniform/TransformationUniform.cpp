#include "TransformationUniform.h"

namespace frieren_core {
    TransformationUniform::TransformationUniform(WGPUDevice device)
        : SimpleTypeUniform(device, get_shader_property_layout_builder(), "transformation_uniform")
    {}

    ShaderPropertyLayoutBuilder TransformationUniform::get_shader_property_layout_builder() {
        ShaderPropertyLayoutBuilder builder{};
        builder.add_entry("model_matrix", ShaderPropertyType::Mat4x4);
        return std::move(builder);
    }

    void TransformationUniform::update_per_object(WGPUDevice device, WGPUQueue queue) const {
        int offset = this->uniform_layout.get_item_offset("model_matrix").value();
        int size = this->uniform_layout.get_item_size("model_matrix").value();
        wgpuQueueWriteBuffer(queue, this->uniform_buffer, offset, &this->model_matrix, size);
    }
}