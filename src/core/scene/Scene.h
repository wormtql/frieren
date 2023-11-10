#ifndef FRIEREN_CORE_SCENE_H
#define FRIEREN_CORE_SCENE_H

#include "GameObjectManager.h"
#include "GameObject.h"
#include <common_include.h>
#include <components/Component.h>

using namespace std;

namespace frieren_core {
    class Scene {
    public:
        string name;
        string id;
    public:
        GameObjectManager game_object_manager;

        Scene();
        explicit Scene(const string& name);

        friend void from_json(const json& j, Scene& scene);

        template<typename T>
        vector<shared_ptr<T>> get_game_objects_of_type() const;
    };

    template<typename T>
    vector<shared_ptr<T>> Scene::get_game_objects_of_type() const {
        static_assert(std::is_base_of<component::Component, T>::value, "T is not a derived type of Component");
        vector<shared_ptr<T>> results;
        for (const auto& go: game_object_manager.get_game_objects()) {
            optional<shared_ptr<T>> comp = go.second->get_component<T>();
            if (comp.has_value()) {
                results.push_back(comp.value());
            }
        }
        return std::move(results);
    }
}

#endif
