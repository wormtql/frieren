#ifndef FRIEREN_CORE_COMPONENT_H
#define FRIEREN_CORE_COMPONENT_H

#include <webgpu/webgpu.hpp>
#include <common_include.h>
#include <utilities/LazyRef.h>
#include <mesh/MeshManager.h>
#include <material/MaterialManager.h>

using namespace std;
using frieren_core::utils::LazyWeakRef;
using frieren_core::utils::LazyRef;
using frieren_core::MeshManager;

namespace frieren_core {
    class GameObject;

    namespace component {
        class Component {
        protected:
            string id;
            LazyWeakRef<GameObject> game_object;
        public:
            Component();
            virtual ~Component() = default;

            virtual void link_referenced_components(const map<string, shared_ptr<Component>>& components) {}
            virtual void link_referenced_mesh(MeshManager& mesh_manager) {}
            virtual void link_referenced_material(WGPUDevice device, WGPUQueue queue, MaterialManager& material_manager) {}

            optional<shared_ptr<GameObject>> get_game_object() const;

            const string& get_id() const;
        };
    }
}

#endif
