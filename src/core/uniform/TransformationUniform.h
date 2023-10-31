#ifndef FRIEREN_CORE_TRANSFORMATION_UNIFORM_H
#define FRIEREN_CORE_TRANSFORMATION_UNIFORM_H

#include <common_include_glm.h>
#include <common_include.h>
#include "Uniform.h"
#include <shader/ShaderProperty.h>

namespace frieren_core {
    class TransformationUniform: public SimpleTypeUniform {
    public:
        glm::mat4x4 model_matrix;
    public:
        static ShaderPropertyLayoutBuilder get_shader_property_layout_builder();

        TransformationUniform(WGPUDevice device);

        void update_per_object(WGPUDevice device, WGPUQueue queue) const override;
    };
}

#endif