#ifndef FRIEREN_CORE_GAME_OBJECT_MANAGER_H
#define FRIEREN_CORE_GAME_OBJECT_MANAGER_H

#include <common_include.h>
#include "GameObject.h"

namespace frieren_core {
    // a single instance has only one game object manager
    class GameObjectManager {
    private:
        map<string, shared_ptr<GameObject>> game_objects;
    public:
        GameObjectManager() = default;

        shared_ptr<GameObject> create_game_object(const string& name);

        void remove_game_object(const string& name);

        optional<shared_ptr<GameObject>> get_game_object(const string& id);

        // only used when deserializing things
        void add_game_object(shared_ptr<GameObject> go);

        const map<string, shared_ptr<GameObject>>& get_game_objects() const {
            return game_objects;
        }

        map<string, shared_ptr<GameObject>>& get_game_objects_mut() {
            return game_objects;
        }
    };
}

#endif
