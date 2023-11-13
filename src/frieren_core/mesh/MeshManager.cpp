#include "MeshManager.h"
#include "BuiltinMesh.h"

namespace frieren_core {
    optional<shared_ptr<Mesh>> MeshManager::get_mesh(const string& id) const {
        if (meshes.find(id) == meshes.end()) {
            return {};
        }

        return meshes.at(id);
    }

    optional<shared_ptr<Mesh>> MeshManager::get_mesh_with_gpu(WGPUDevice device, WGPUQueue queue, const string& id) {
        if (meshes.find(id) == meshes.end()) {
            return {};
        }

        auto m = meshes.at(id);
        if (!m->is_gpu_buffer_created()) {
            m->create_gpu_buffer(device);
            m->write_gpu_buffer(queue);
        }

        return m;
    }

    void MeshManager::init_builtin_mesh(WGPUDevice device, WGPUQueue queue) {
        Mesh cube = BuiltinMesh::create_cube_mesh(1.0f);
        cube.create_gpu_buffer(device);
        cube.write_gpu_buffer(queue);
        string id = cube.id;
        meshes[id] = make_shared<Mesh>(std::move(cube));

        gpu_index_buffer_used += cube.indices.size() * sizeof(decltype(cube.indices)::value_type);
        gpu_vertex_buffer_used += cube.vertices.size() * sizeof(decltype(cube.vertices)::value_type);

        // todo add more builtin meshes
    }
}
