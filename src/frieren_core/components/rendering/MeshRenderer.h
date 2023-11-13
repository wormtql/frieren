#ifndef FRIEREN_CORE_COMPONENT_MESH_RENDERER_H
#define FRIEREN_CORE_COMPONENT_MESH_RENDERER_H

#include <components/Component.h>
#include <vector>
#include <material/Material.h>
#include <memory>
#include <utilities/LazyRef.h>

using namespace std;
using frieren_core::utils::LazyRef;

namespace frieren_core::component {
    class MeshRenderer: public Component {
    private:
        vector<LazyRef<Material>> materials;
    public:
        MeshRenderer();

        friend void from_json(const json& j, MeshRenderer& mesh_renderer);

        void link_referenced_material(WGPUDevice device, WGPUQueue queue, MaterialManager& material_manager) override;

        const vector<LazyRef<Material>>& get_materials() const {
            return materials;
        } 
    };
}

#endif
