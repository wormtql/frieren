#ifndef FRIEREN_CORE_VERTEX_H
#define FRIEREN_CORE_VERTEX_H

#include <common_include_glm.h>
#include <webgpu/webgpu.hpp>
#include <common_include.h>

namespace frieren_core {
    struct WGPUVertexBufferLayoutOwned {
        uint64_t arrayStride;
        WGPUVertexStepMode stepMode;
        std::vector<WGPUVertexAttribute> attributes;

        WGPUVertexBufferLayout get_wgpu_vertex_buffer_layout();
    };

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 uv;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        static WGPUVertexBufferLayoutOwned get_vertex_buffer_layout();
    };
}

#endif