#include "ComponentRegistry.h"
#include "components/all_components.h"

namespace frieren_core::component {
    ComponentRegistry ComponentRegistry::instance{};

    ComponentRegistry::ComponentRegistry() {
        init_deserializer();
    }

    void ComponentRegistry::init_deserializer() {
        add_deserializer<Transform>();
        add_deserializer<MeshRenderer>();
    }

    optional<shared_ptr<Component>> ComponentRegistry::deserialize_component(const json &j) {
        assert(j.is_object());

        string key_name;
        for (const auto& i: j.items()) {
            key_name = i.key();
            break;
        }

        if (creator_registry.find(key_name) == creator_registry.end()) {
            return {};
        }

        return creator_registry[key_name](j[key_name]);
    }
}
