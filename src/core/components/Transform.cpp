#include "Transform.h"

namespace frieren_core::component {
    Transform::Transform(): Component() {

    }

    void Transform::add_children(shared_ptr<Transform> parent, shared_ptr<Transform> child) {
        parent->children.push_back(child);
        child->parent = parent;

        child->cached_transform_matrix = parent->get_transform_matrix() * child->get_transform_matrix_local();
    }

    glm::mat4x4 Transform::get_transform_matrix() const {
        if (this->has_parent()) {
            return cached_transform_matrix;
        } else {
            return get_transform_matrix_local();
        }
    }

    glm::mat4x4 Transform::get_transform_matrix_local() const {
        glm::mat4x4 ret;
        ret = ret * this->scale;
        ret = glm::toMat4(this->rotation) * ret;
        ret = glm::translate(ret, this->position);

        return ret;
    }

    bool Transform::has_parent() const {
        if (auto x = parent.lock()) {
            return true;
        }
        return false;
    }
}
