#ifndef FRIEREN_CORE_MESH_MANAGER
#define FRIEREN_CORE_MESH_MANAGER

#include <common_include.h>
#include "Mesh.h"
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    class MeshManager {
    private:
        map<string, shared_ptr<Mesh>> meshes;

        int64_t gpu_vertex_buffer_used = 0;
        int64_t gpu_index_buffer_used = 0;
    public:
        MeshManager() = default;

        optional<shared_ptr<Mesh>> get_mesh(const string& name) const;

        optional<shared_ptr<Mesh>> get_mesh_with_gpu(WGPUDevice device, WGPUQueue queue, const string& name);

        void init_builtin_mesh(WGPUDevice device, WGPUQueue queue);
    };
}

#endif
