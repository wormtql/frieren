#ifndef FRIEREN_CORE_COMPONENT_TRANSFORM_H
#define FRIEREN_CORE_COMPONENT_TRANSFORM_H

#include "Component.h"
#include <common_include_glm.h>
#include <common_include.h>
#include <utilities/LazyRef.h>

using namespace std;

namespace frieren_core::component {
    class Transform: public Component {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        float scale;

        vector<utils::LazyRef<Transform>> children;
        utils::LazyWeakRef<Transform> parent;
    public:

        Transform();

        static void add_child(shared_ptr<Transform> parent, shared_ptr<Transform> child);

        void remove_child(shared_ptr<Transform> child);

        glm::mat4x4 get_transform_matrix() const;

        glm::mat4x4 get_transform_matrix_local() const;

        bool has_parent() const;

        optional<shared_ptr<Transform>> get_parent() const;

        void link_referenced_components(const map<string, shared_ptr<Component>>& components) override;

        friend void from_json(const json& j, Transform& transform);
    };
}

#endif