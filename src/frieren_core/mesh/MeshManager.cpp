#include "MeshManager.h"
#include "BuiltinMesh.h"
#include <utilities/utils.h>

namespace frieren_core {
    optional<shared_ptr<Mesh>> MeshManager::get_mesh(const string& id) {
        assert(device);
        assert(queue);

        if (meshes.find(id) != meshes.end()) {
            return meshes.at(id);
        }

        if (loaded_descriptors.find(id) == loaded_descriptors.end()) {
            return {};
        }

        shared_ptr<Mesh> m = make_shared<Mesh>(device, queue, loaded_descriptors.at(id));
        meshes[id] = m;
        this->gpu_vertex_buffer_used += sizeof(Vertex) * m->vertices.size();
        this->gpu_index_buffer_used += sizeof(uint32_t) * m->indices.size();

        return m;
    }

    // optional<shared_ptr<Mesh>> MeshManager::get_mesh_with_gpu(WGPUDevice device, WGPUQueue queue, const string& id) {
    //     if (meshes.find(id) == meshes.end()) {
    //         return {};
    //     }

    //     auto m = meshes.at(id);
    //     if (!m->is_gpu_buffer_created()) {
    //         m->create_gpu_buffer(device);
    //         m->write_gpu_buffer(queue);
    //     }

    //     return m;
    // }

    void MeshManager::add_search_path(const filesystem::path& path) {
        this->search_paths.emplace_back(path);
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

    void MeshManager::load_mesh_descriptor(const filesystem::path& path) {
        if (filesystem::exists(path)) {
            string json_string = utils::read_file_to_string(path).value();
            json j = json::parse(json_string);
            MeshDescriptor mesh_desc = j.template get<MeshDescriptor>();

            auto mesh_path = path.parent_path() / mesh_desc.mesh_path;
            mesh_desc.mesh_path = mesh_path.string();

            loaded_descriptors[mesh_desc.id] = mesh_desc;
        }
    }

    void MeshManager::init_all_descriptors() {
        for (const auto& search_path: this->search_paths) {
            for (const auto& dir_entry: filesystem::recursive_directory_iterator(search_path)) {
                if (dir_entry.is_regular_file()) {
                    filesystem::path p = dir_entry.path();
                    filesystem::path filename = p.filename();
                    auto name = filename.string();
                    if (name.ends_with(".mesh.json")) {
                        load_mesh_descriptor(p);
                    }
                }
            }
        }
    }
}
