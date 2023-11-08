#include "Mesh.h"
#include <webgpu/webgpu.hpp>
#include <nanoid/nanoid.h>

namespace frieren_core {
    Mesh::Mesh(const string& name): name(name) {
        this->id = nanoid::generate();
    }

    bool Mesh::is_gpu_buffer_created() {
        return vertex_buffer != nullptr && index_buffer != nullptr;
    }

    void Mesh::create_gpu_buffer(WGPUDevice device) {
        if (vertex_buffer) {
            wgpuBufferDestroy(vertex_buffer);
            wgpuBufferRelease(vertex_buffer);
            vertex_buffer = nullptr;
        }
        if (index_buffer) {
            wgpuBufferDestroy(index_buffer);
            wgpuBufferRelease(index_buffer);
            index_buffer = nullptr;
        }

        WGPUBufferDescriptor vertex_buffer_desc;
        vertex_buffer_desc.nextInChain = nullptr;
        vertex_buffer_desc.size = vertices.size() * sizeof(Vertex);
        vertex_buffer_desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex;
        vertex_buffer_desc.mappedAtCreation = false;
        vertex_buffer = wgpuDeviceCreateBuffer(device, &vertex_buffer_desc);

        WGPUBufferDescriptor index_buffer_desc;
        index_buffer_desc.nextInChain = nullptr;
        index_buffer_desc.size = indices.size() * sizeof(uint32_t);
        index_buffer_desc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index;
        index_buffer_desc.mappedAtCreation = false;
        index_buffer = wgpuDeviceCreateBuffer(device, &index_buffer_desc);
    }

    void Mesh::write_gpu_buffer(WGPUQueue queue) {
        wgpuQueueWriteBuffer(queue, vertex_buffer, 0, vertices.data(), vertices.size() * sizeof(Vertex));
        wgpuQueueWriteBuffer(queue, index_buffer, 0, indices.data(), indices.size() * sizeof(uint32_t));
    }

    void Mesh::set_buffer_for_render_pass(WGPURenderPassEncoder render_pass) const {
        wgpuRenderPassEncoderSetVertexBuffer(render_pass, 0, vertex_buffer, 0, sizeof(Vertex) * vertices.size());
        wgpuRenderPassEncoderSetIndexBuffer(render_pass, index_buffer, WGPUIndexFormat_Uint32, 0, sizeof(uint32_t) * indices.size());
    }
}
