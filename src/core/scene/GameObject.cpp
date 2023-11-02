#include "GameObject.h"
#include <nanoid/nanoid.h>
#include <components/ComponentRegistry.h>
#include <utilities/utils.h>

using namespace frieren_core::component;

namespace frieren_core {
    GameObject::GameObject(const string& name): name(name) {
        this->id = nanoid::generate();
    }

    GameObject::GameObject() {
        id = nanoid::generate();
    }

    const string& GameObject::get_id() const {
        return id;
    }

    const string& GameObject::get_name() const {
        return name;
    }

    const map<string, shared_ptr<Component>>& GameObject::get_components() const {
        return components;
    }
}

// serde
namespace frieren_core {
    void from_json(const json& j, GameObject& go) {
        go.id = j["id"];
        go.name = j["name"];
        if (j.contains("components")) {
            for (const auto& item: j["components"]) {
                optional<shared_ptr<Component>> comp = ComponentRegistry::instance.deserialize_component(item);
                string component_name = utils::get_json_object_any_key(item);
                if (comp.has_value()) {
                    go.components[component_name] = comp.value();
                }
            }
        }
    }
}
