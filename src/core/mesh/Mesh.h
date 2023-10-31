#ifndef FRIEREN_CORE_MESH_H
#define FRIEREN_CORE_MESH_H

#include <webgpu/webgpu.hpp>
#include "Vertex.h"

namespace frieren_core {
    class Mesh {
    private:
        WGPUBuffer vertex_buffer = nullptr;
        WGPUBuffer index_buffer = nullptr;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    public:
        Mesh() = default;

        bool is_gpu_buffer_created();

        void create_gpu_buffer(WGPUDevice device);
    };
}

#endif