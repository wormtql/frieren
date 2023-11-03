#include "MeshManager.h"
#include "BuiltinMesh.h"

namespace frieren_core {
    optional<shared_ptr<Mesh>> MeshManager::get_mesh(const string& name) const {
        if (meshes.find(name) == meshes.end()) {
            return {};
        }

        return meshes.at(name);
    }

    optional<shared_ptr<Mesh>> MeshManager::get_mesh_with_gpu(WGPUDevice device, WGPUQueue queue, const string& name) {
        if (meshes.find(name) == meshes.end()) {
            return {};
        }

        auto m = meshes.at(name);
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
        meshes[cube.name] = make_shared<Mesh>(std::move(cube));

        gpu_index_buffer_used += cube.indices.size() * sizeof(decltype(cube.indices)::value_type);
        gpu_vertex_buffer_used += cube.vertices.size() * sizeof(decltype(cube.vertices)::value_type);

        // todo add more builtin meshes
    }
}
