#ifndef FRIEREN_CORE_COMPONENT_REGISTRY_H
#define FRIEREN_CORE_COMPONENT_REGISTRY_H

#include <common_include.h>
#include <components/Component.h>
#include <components/Transform.h>

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

        string full_name = typeid(T).name();
        auto it = full_name.find_last_of("::");
        string classname;
        if (it == string::npos) {
            classname = full_name;
        } else {
            classname = full_name.substr(it + 2);
        }

        auto f = [] (const json& j) {
            shared_ptr<T> ret = make_shared<T>();
            from_json(j, *ret.get());
            return dynamic_pointer_cast<Component, T>(ret);
        };

        creator_registry[classname] = f;
    }
}

#endif
