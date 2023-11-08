#ifndef FRIEREN_CORE_GAME_OBJECT_H
#define FRIEREN_CORE_GAME_OBJECT_H

#include <components/Component.h>
#include <components/Transform.h>
#include <common_include.h>
#include <utilities/LazyRef.h>
#include <utilities/utils.h>

using namespace std;
using frieren_core::component::Component;
using frieren_core::component::Transform;
using frieren_core::utils::LazyRef;

namespace frieren_core {
    class GameObject {
    private:
        string id;
        string name;

        map<string, shared_ptr<Component>> components;
    public:
        explicit GameObject(const string& name);
        GameObject();

        template<typename T>
        std::optional<shared_ptr<T>> get_component();

        template<typename T>
        std::optional<shared_ptr<T>> remove_component();

        template<typename T>
        void add_component(shared_ptr<T> component);

        [[nodiscard]] const string& get_id() const;

        [[nodiscard]] const string& get_name() const;

        [[nodiscard]] const map<string, shared_ptr<Component>>& get_components() const;

        [[nodiscard]] map<string, shared_ptr<Component>>& get_components_mut();

        friend void from_json(const json& j, GameObject& go);
    };

    // void from_json(const json& j, GameObject& go);

    template<typename T>
    void GameObject::add_component(shared_ptr<T> component) {
        static_assert(std::is_base_of<Component, T>::value, "can only add to game object with component");
        
        string name = utils::get_type_name<T>();
        components[name] = component;
    }

    template<typename T>
    std::optional<shared_ptr<T>> GameObject::get_component() {
        static_assert(std::is_base_of<Component, T>::value, "can only get component");

        string name = utils::get_type_name<T>();
        if (components.find(name) != components.end()) {
            auto temp = components[name];
            return dynamic_pointer_cast<T, Component>(temp);
        }
        return {};
    }

    template<typename T>
    std::optional<shared_ptr<T>> GameObject::remove_component() {
        static_assert(std::is_base_of<Component, T>::value, "can only remove component");

        // we cannot remove transform component
        if (std::is_same<T, Transform>::value) {
            return {};
        }

        string name = utils::get_type_name<T>();
        auto it = components.find(name);
        if (it != components.end()) {
            shared_ptr<T> comp = components[name];
            components.erase(it);
            return comp;
        }
        return {};
    }
}

#endif
