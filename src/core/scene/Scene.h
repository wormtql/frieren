#ifndef FRIEREN_CORE_SCENE_H
#define FRIEREN_CORE_SCENE_H

#include "GameObjectManager.h"
#include "GameObject.h"
#include <common_include.h>

using namespace std;

namespace frieren_core {
    class Scene {
    private:
        string name;
        string id;
    public:
        GameObjectManager game_object_manager;

        Scene();
        explicit Scene(const string& name);

        friend void from_json(const json& j, Scene& scene);
    };
}

#endif
