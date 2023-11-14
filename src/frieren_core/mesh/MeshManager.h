#ifndef FRIEREN_CORE_MESH_MANAGER
#define FRIEREN_CORE_MESH_MANAGER

#include <common_include.h>
#include "Mesh.h"
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    class MeshManager {
    private:
        map<string, MeshDescriptor> loaded_descriptors;
        map<string, shared_ptr<Mesh>> meshes;

        vector<filesystem::path> search_paths;

        int64_t gpu_vertex_buffer_used = 0;
        int64_t gpu_index_buffer_used = 0;

        void load_mesh_descriptor(const filesystem::path& path);
    public:
        WGPUDevice device;
        WGPUQueue queue;

        MeshManager() = default;

        void add_search_path(const filesystem::path& path);

        void init_all_descriptors();

        optional<shared_ptr<Mesh>> get_mesh(const string& id);

        // optional<shared_ptr<Mesh>> get_mesh_with_gpu(WGPUDevice device, WGPUQueue queue, const string& id);

        void init_builtin_mesh(WGPUDevice device, WGPUQueue queue);
    };
}

#endif
