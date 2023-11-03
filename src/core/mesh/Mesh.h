#ifndef FRIEREN_CORE_MESH_H
#define FRIEREN_CORE_MESH_H

#include <webgpu/webgpu.hpp>
#include "Vertex.h"
#include <string>

using namespace std;

namespace frieren_core {
    class Mesh {
    public:
        WGPUBuffer vertex_buffer = nullptr;
        WGPUBuffer index_buffer = nullptr;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    public:
        string name;
        string id;

        explicit Mesh(const string& name);

        bool is_gpu_buffer_created();

        void create_gpu_buffer(WGPUDevice device);

        void write_gpu_buffer(WGPUQueue queue);
    };
}

#endif