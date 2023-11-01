#ifndef FRIEREN_CORE_GAME_OBJECT_H
#define FRIEREN_CORE_GAME_OBJECT_H

#include <components/Component.h>
#include <common_include.h>

using namespace std;
using frieren_core::component::Component;

namespace frieren_core {
    class GameObject {
    private:
        int id;
        string name;
        static int next_id;

        map<std::type_info, shared_ptr<Component>> components;
    public:
        GameObject(const string& name);

        template<typename T>
        std::optional<shared_ptr<T>> get_component();

        template<typename T>
        std::optional<shared_ptr<T>> remove_component();

        template<typename T>
        void add_component(shared_ptr<T> component);
    };

    template<typename T>
    void add_component(shared_ptr<T> component) {
        static_assert(std::is_base_of<Component, T>::value, "can only add to game object with component");
        const auto& ty = typeid(T);
        components[ty] = component;
    }

    template<typename T>
    std::optional<shared_ptr<T>> GameObject::get_component() {
        static_assert(std::is_base_of<Component, T>::value, "can only get component");
        const auto& ty = typeid(T);
        if (components.find(ty) != components.end()) {
            return components[ty];
        }
        return {};
    }

    template<typename T>
    std::optional<shared_ptr<T>> GameObject::remove_component() {
        static_assert(std::is_base_of<Component, T>::value, "can only remove component");
        const auto& ty = typeid(T);
        auto it = components.find(ty);
        if (it != components.end()) {
            shared_ptr<T> comp = components[ty];
            components.erase(it);
            return comp;
        }
        return {};
    }
}

#endif
