#ifndef FRIEREN_CORE_GAME_OBJECT_H
#define FRIEREN_CORE_GAME_OBJECT_H

#include <components/Component.h>
#include <common_include.h>
#include <utilities/LazyRef.h>

using namespace std;
using frieren_core::component::Component;
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

        friend void from_json(const json& j, GameObject& go);
    };

    // void from_json(const json& j, GameObject& go);

    template<typename T>
    void GameObject::add_component(shared_ptr<T> component) {
        static_assert(std::is_base_of<Component, T>::value, "can only add to game object with component");
        const auto& ty = typeid(T);
        components[ty.name()] = component;
    }

    template<typename T>
    std::optional<shared_ptr<T>> GameObject::get_component() {
        static_assert(std::is_base_of<Component, T>::value, "can only get component");
        const auto& ty = typeid(T);
        if (components.find(ty.name()) != components.end()) {
            auto temp = components[ty.name()];
            return dynamic_pointer_cast<T, Component>(temp);
        }
        return {};
    }

    template<typename T>
    std::optional<shared_ptr<T>> GameObject::remove_component() {
        static_assert(std::is_base_of<Component, T>::value, "can only remove component");
        const auto& ty = typeid(T);
        auto it = components.find(ty.name());
        if (it != components.end()) {
            shared_ptr<T> comp = components[ty.name()];
            components.erase(it);
            return comp;
        }
        return {};
    }
}

#endif
