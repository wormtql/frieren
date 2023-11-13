#ifndef FRIEREN_CORE_COMPONENT_REGISTRY_H
#define FRIEREN_CORE_COMPONENT_REGISTRY_H

#include <common_include.h>
#include <components/Component.h>
#include <components/Transform.h>
#include <utilities/utils.h>

using namespace std;

namespace frieren_core::component {
    class ComponentRegistry {
    public:
        static ComponentRegistry instance;

        using ComponentCreator = function<optional<shared_ptr<Component>>(const json&)>;
        map<string, ComponentCreator> creator_registry;

        void init_deserializer();
    public:
        ComponentRegistry();

        template<typename T>
        void add_deserializer();

        optional<shared_ptr<Component>> deserialize_component(const json& j);
    };

    template<typename T>
    void ComponentRegistry::add_deserializer() {
        static_assert(std::is_base_of<Component, T>::value);

        string classname = utils::get_type_name<T>();

        auto f = [] (const json& j) {
            shared_ptr<T> ret = make_shared<T>();
            from_json(j, *ret.get());
            return dynamic_pointer_cast<Component, T>(ret);
        };

        creator_registry[classname] = f;
    }
}

#endif
