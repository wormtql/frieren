#ifndef FRIEREN_CORE_COMPONENT_TRANSFORM_H
#define FRIEREN_CORE_COMPONENT_TRANSFORM_H

#include "Component.h"
#include <common_include_glm.h>
#include <common_include.h>

using namespace std;

namespace frieren_core::component {
    class Transform: public Component {
    private:
        glm::vec3 position;
        glm::quat rotation;
        float scale;

        vector<shared_ptr<Transform>> children;
        weak_ptr<Transform> parent;

        glm::mat4x4 cached_transform_matrix;
    public:
        Transform();

        static void add_children(shared_ptr<Transform> parent, shared_ptr<Transform> child);

        glm::mat4x4 get_transform_matrix() const;

        glm::mat4x4 get_transform_matrix_local() const;

        bool has_parent() const;
    };
}

#endif