#include "CameraUniform.h"

namespace frieren_core {
    ShaderPropertyLayoutBuilder CameraUniform::get_shader_property_layout_builer() {
        ShaderPropertyLayoutBuilder builder{};
        builder.add_entry("world_space_camera_position", ShaderPropertyType::Float4);
        builder.add_entry("view_matrix", ShaderPropertyType::Mat4x4);
        builder.add_entry("projection_matrix", ShaderPropertyType::Mat4x4);
        builder.add_entry("projection_params", ShaderPropertyType::Float4);
        return std::move(builder);
    }

    CameraUniform::CameraUniform(WGPUDevice device)
        : SimpleTypeUniform(device, get_shader_property_layout_builer(), "camera_uniform") {

    }

    void CameraUniform::update_per_frame(WGPUDevice device, WGPUQueue queue) const {
        int property_offset = this->uniform_layout.get_item_offset("world_space_camera_position").value();
        int property_size = this->uniform_layout.get_item_size("world_space_camera_position").value();
        wgpuQueueWriteBuffer(queue, this->uniform_buffer, property_offset, &this->world_space_camera_position, sizeof(glm::vec4));

        property_offset = this->uniform_layout.get_item_offset("view_matrix").value();
        property_size = this->uniform_layout.get_item_size("view_matrix").value();
        wgpuQueueWriteBuffer(queue, this->uniform_buffer, property_offset, &this->view_matrix, property_size);

        property_offset = this->uniform_layout.get_item_offset("projection_matrix").value();
        property_size = this->uniform_layout.get_item_size("projection_matrix").value();
        wgpuQueueWriteBuffer(queue, this->uniform_buffer, property_offset, &this->projection_matrix, property_size);

        property_offset = this->uniform_layout.get_item_offset("projection_params").value();
        property_size = this->uniform_layout.get_item_size("projection_params").value();
        wgpuQueueWriteBuffer(queue, this->uniform_buffer, property_offset, &this->projection_params, property_size);
    }
}