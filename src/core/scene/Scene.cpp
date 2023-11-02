#include "Scene.h"
#include <nanoid/nanoid.h>
#include "GameObject.h"

namespace frieren_core {
    Scene::Scene(const string& name): name(name) {
        id = nanoid::generate();
    }

    Scene::Scene() {
        id = nanoid::generate();
    }


}

// serde
namespace frieren_core {
    void from_json(const json& j, Scene& scene) {
        scene.id = j["id"];
        scene.name = j["name"];

        map<string, shared_ptr<GameObject>> goes;
        map<string, shared_ptr<Component>> components;
        if (j.contains("game_objects")) {
            for (const auto& item: j["game_objects"]) {
                GameObject go = item.template get<GameObject>();
                shared_ptr<GameObject> go2 = make_shared<GameObject>(std::move(go));
                goes[go2->get_name()] = go2;

                for (const auto& comp: go2->get_components()) {
                    components[comp.second->get_id()] = comp.second;
                }
            }
        }

        const auto& components_ref = components;
        for (auto& item: components) {
            item.second->link_referenced_components(components_ref);
        }
    }
}