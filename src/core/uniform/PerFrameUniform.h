#ifndef FRIEREN_CORE_PER_FRAME_UNIFORM_H
#define FRIEREN_CORE_PER_FRAME_UNIFORM_H

#include <common_include_glm.h>
#include <common_include.h>
#include <webgpu/webgpu.hpp>

namespace frieren_core {
    struct PerFrameUniformData {
        // camera related uniforms
        glm::vec4 world_space_camera_position;
        glm::mat4x4 view_matrix;
        glm::mat4x4 projection_matrix;
        // same as unity
        glm::vec4 projection_params;
    };

    class PerFrameUniform {
    private:
        WGPUBuffer buffer = nullptr;
    public:
        PerFrameUniformData data;

        PerFrameUniform() = default;
        ~PerFrameUniform();

        void create_wgpu_buffer(WGPUDevice device);
        
        void update_uniform_buffer(WGPUQueue queue);
    };
}

#endif