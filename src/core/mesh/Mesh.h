#ifndef FRIEREN_CORE_MESH_H
#define FRIEREN_CORE_MESH_H

#include <webgpu/webgpu.hpp>

namespace frieren_core {
    struct Mesh {
        WGPUBuffer vertex_buffer;
        WGPUBuffer index_buffer;

        static WGPUVertexBufferLayout get_vertex_buffer_layout();
    };
}

#endif