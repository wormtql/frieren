#include "MeshRenderer.h"

namespace frieren_core::component {
    MeshRenderer::MeshRenderer(): Component() {

    }

    void from_json(const json& j, MeshRenderer& mesh_renderer) {
        mesh_renderer.id = j["id"];
        if (j.contains("materials")) {
            for (const auto& item: j["materials"]) {
                string mat_id = item.template get<string>();
                mesh_renderer.materials.emplace_back(mat_id);
            }
        }
    }

    void MeshRenderer::link_referenced_material(WGPUDevice device, WGPUQueue queue, MaterialManager& material_manager) {
        for (auto& mat: this->materials) {
            if (mat.object == nullptr) {
                auto result = material_manager.get_material(device, queue, mat.referenced_id).value();
                mat.object = result;
            }
        }
    }
}