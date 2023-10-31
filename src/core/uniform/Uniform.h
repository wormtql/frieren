#ifndef FRIEREN_CORE_UNIFORM_H
#define FRIEREN_CORE_UNIFORM_H

#include <webgpu/webgpu.hpp>
#include <shader/ShaderProperty.h>
#include <string>

namespace frieren_core {
    class SimpleTypeUniform {
    protected:
        WGPUBuffer uniform_buffer;
        int size;
        ShaderPropertyLayout uniform_layout;
        std::string label;
    public:
        SimpleTypeUniform(WGPUDevice device, const ShaderPropertyLayoutBuilder layout_builder, const std::string& label);
        ~SimpleTypeUniform();

        virtual void update_per_frame(WGPUDevice device, WGPUQueue queue) const;

        virtual void update_per_object(WGPUDevice device, WGPUQueue queue) const;
    };
}

#endif
