#ifndef FRIEREN_CORE_COMPONENT_MESH_FILTER_H
#define FRIEREN_CORE_COMPONENT_MESH_FILTER_H

#include <components/Component.h>
#include <mesh/Mesh.h>
#include <common_include.h>
#include <utilities/LazyRef.h>

using namespace std;
using frieren_core::Mesh;
using frieren_core::utils::LazyRef;

namespace frieren_core::component {
    class MeshFilter: public Component {
    private:
        LazyRef<Mesh> mesh;
    public:
        MeshFilter();

        friend void from_json(const json& j, MeshFilter& mesh_filter);

        void link_referenced_mesh(MeshManager& mesh_manager) override;
    };
}

#endif
