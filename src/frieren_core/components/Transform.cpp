#include "Transform.h"
#include <glm_serde.h>

using frieren_core::utils::LazyRef;
using frieren_core::utils::LazyWeakRef;

namespace frieren_core::component {
    Transform::Transform(): Component() {
        position = glm::vec3(0, 0, 0);
        rotation = glm::vec3(0, 0, 0);
        scale = 1;
    }

    void Transform::add_child(shared_ptr<Transform> parent, shared_ptr<Transform> child) {
        frieren_core::utils::LazyRef<Transform> c;
        c.referenced_id = child->id;
        c.object = child;
        parent->children.push_back(c);
        
        utils::LazyWeakRef<Transform> w;
        w.referenced_id = parent->id;
        w.object = parent;
        child->parent = w;
    }

    glm::mat4x4 Transform::get_transform_matrix() const {
        if (this->has_parent()) {
            return parent.get().lock()->get_transform_matrix() * get_transform_matrix_local();
        } else {
            return get_transform_matrix_local();
        }
    }

    glm::mat4x4 Transform::get_transform_matrix_local() const {
        glm::mat4x4 ret;
        ret = ret * this->scale;
        ret = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z) * ret;
        ret = glm::translate(ret, this->position);

        return ret;
    }

    bool Transform::has_parent() const {
        return !parent.object.expired();
    }

    bool Transform::has_children() const {
        return this->children.size() > 0;
    }

    optional<shared_ptr<Transform>> Transform::get_parent() const {
        if (shared_ptr<Transform> p = parent.object.lock()) {
            return p;
        }
        return {};
    }

    void Transform::remove_child(shared_ptr<Transform> child) {
        child->parent.object.reset();
        for (auto it = children.begin(); it != children.end(); it++) {
            if ((*it).object == child) {
                children.erase(it);
                break;
            }
        }
    }

    void Transform::link_referenced_components(const map<string, shared_ptr<Component>>& components) {
        // link children
        for (auto& child: this->children) {
            shared_ptr<Component> obj = components.at(child.referenced_id);
            shared_ptr<Transform> transform = dynamic_pointer_cast<Transform, Component>(obj);
            child.object = transform;
        }

        // link parent
        if (parent.referenced_id != "") {
            shared_ptr<Component> p1 = components.at(parent.referenced_id);
            shared_ptr<Transform> p2 = dynamic_pointer_cast<Transform, Component>(p1);
            parent.object = p2;
        }
    }
}

// serde
namespace frieren_core::component {
    void from_json(const json& j, Transform& transform) {
        transform.id = j["id"];
        transform.position = j["position"].template get<glm::vec3>();
        transform.rotation = j["rotation"].template get<glm::vec3>();
        transform.scale = j["scale"];
        if (j.contains("children")) {
            for (const auto& c: j["children"]) {
                string id = c.template get<string>();
                transform.children.emplace_back(id);
            }
        }
        if (j.contains("parent")) {
            string id = j["parent"].template get<string>();
            transform.parent = LazyWeakRef<Transform>{id};
        }
    }
}
