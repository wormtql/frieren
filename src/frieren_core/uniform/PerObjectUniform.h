#ifndef FRIEREN_CORE_PER_OBJECT_UNIFORM_H
#define FRIEREN_CORE_PER_OBJECT_UNIFORM_H

#include <common_include_glm.h>
#include <common_include.h>
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    struct PerObjectUniformData {
        glm::mat4x4 frieren_object_matrix;
    };

    class PerObjectUniform {
    public:
        WGPUBuffer buffer = nullptr;
        PerObjectUniformData data;
    public:
        PerObjectUniform() = default;
        PerObjectUniform(const PerObjectUniform& other) = delete;
        ~PerObjectUniform();

        void create_wgpu_buffer(WGPUDevice device);

        void update_uniform_buffer(WGPUQueue queue);

        WGPUBuffer get_buffer() const {
            return buffer;
        }
    };
}

#endif
