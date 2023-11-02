#include "GameObjectManager.h"
#include <components/Transform.h>

using namespace frieren_core::component;

namespace frieren_core {
    shared_ptr<GameObject> GameObjectManager::create_game_object(const string& name) {
        shared_ptr<GameObject> go = make_shared<GameObject>(name);

        shared_ptr<Transform> transform = make_shared<Transform>();
        go->add_component<Transform>(transform);

        game_objects[go->get_id()] = go;
        return go;
    }

    void GameObjectManager::remove_game_object(const string& name) {
        auto it = game_objects.find(name);
        if (it == game_objects.end()) {
            return;
        }
        shared_ptr<GameObject> go = (*it).second;
        game_objects.erase(it);

        shared_ptr<Transform> transform = go->get_component<Transform>().value();

        // if it has a parent, then remove from parent
        if (transform->has_parent()) {
            shared_ptr<Transform> parent = transform->get_parent().value();
            parent->remove_child(transform);
        }

        // if it has children, don't do anything
    }

    optional<shared_ptr<GameObject>> GameObjectManager::get_game_object(const string& name) {
        if (game_objects.find(name) == game_objects.end()) {
            return {};
        }
        return game_objects[name];
    }

    void GameObjectManager::add_game_object(shared_ptr<GameObject> go) {
        game_objects[go->get_id()] = go;
    }
}