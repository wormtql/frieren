#include "Mesh.h"
#include <webgpu/webgpu.hpp>

namespace frieren_core {

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
}