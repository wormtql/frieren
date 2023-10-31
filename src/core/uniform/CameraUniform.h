#ifndef FRIEREN_CORE_CAMERA_UNIFORM_H
#define FRIEREN_CORE_CAMERA_UNIFORM_H

#include <common_include_glm.h>
#include <common_include.h>
#include "Uniform.h"
#include <shader/ShaderProperty.h>

namespace frieren_core {
    class CameraUniform: public SimpleTypeUniform {
    public:
        glm::vec4 world_space_camera_position;
        glm::mat4x4 view_matrix;
        glm::mat4x4 projection_matrix;
        // same as unity
        glm::vec4 projection_params;
    public:
        static ShaderPropertyLayoutBuilder get_shader_property_layout_builer();

        CameraUniform(WGPUDevice device);

        void update_per_frame(WGPUDevice device, WGPUQueue queue) const override;

        void update_per_object(WGPUDevice device, WGPUQueue queue) const override;
    };
}

#endif