#include "MeshFilter.h"

namespace frieren_core::component {
    MeshFilter::MeshFilter(): Component() {

    }

    void MeshFilter::link_referenced_mesh(MeshManager& mesh_manager) {
        this->mesh.object = mesh_manager.get_mesh(mesh.referenced_id).value();
    }

    void from_json(const json& j, MeshFilter& mesh_filter) {
        mesh_filter.id = j["id"];
        if (j.contains("mesh")) {
            mesh_filter.mesh.referenced_id = j["mesh"];
        }
    }
}