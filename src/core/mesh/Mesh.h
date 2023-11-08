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

        /**
         * @brief Set the buffer for render pass object
         * set vertex buffer and index buffer
         * @param render_pass 
         */
        void set_buffer_for_render_pass(WGPURenderPassEncoder render_pass) const;
    
        size_t get_index_count() const {
            return indices.size();
        }
    };
}

#endif