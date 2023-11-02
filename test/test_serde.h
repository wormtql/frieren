#include <catch2/catch_test_macros.hpp>
#include <string>
#include <scene/GameObject.h>
#include <nlohmann/json.hpp>
#include <components/Transform.h>
#include <iostream>
#include <scene/Scene.h>

using json = nlohmann::json;

using namespace std;
using namespace frieren_core;
using namespace frieren_core::component;

TEST_CASE("TEST serde gameobject", "[test1]") {
    string str = R"(
{
    "name": "go",
    "id": "id1",
    "components": [
        {
            "Transform": {
                "id": "transform1",
                "position": [1, 2, 3],
                "rotation": [10, 20, 30],
                "scale": 1
            }
        }
    ]
})";
    json j = json::parse(str);
    GameObject go = j.template get<GameObject>();

    REQUIRE(go.get_id() == "id1");
    
    auto transform = go.get_component<Transform>().value();
    REQUIRE(transform->position.x == 1);
    REQUIRE(transform->children.size() == 0);
    REQUIRE(transform->parent.object.expired());
}

TEST_CASE("TEST serde scene", "[test1]") {
    string str = R"(
{
    "name": "scene",
    "id": "asd",
    "game_objects": [
        {
            "name": "go1",
            "id": "id1",
            "components": [
                {
                    "Transform": {
                        "id": "transform1",
                        "position": [1, 2, 3],
                        "rotation": [10, 20, 30],
                        "scale": 1,
                        "parent": "transform2"
                    }
                }
            ]
        },
        {
            "name": "go2",
            "id": "id2",
            "components": [
                {
                    "Transform": {
                        "id": "transform2",
                        "position": [1, 2, 3],
                        "rotation": [10, 20, 30],
                        "scale": 1,
                        "children": [
                            "transform1"
                        ]
                    }
                }
            ]
        }
    ]
})";
    json j = json::parse(str);
    Scene scene = j.template get<Scene>();

    REQUIRE(scene.id == "asd");
    REQUIRE(scene.game_object_manager.get_game_objects().size() == 2);

    shared_ptr<GameObject> go1 = scene.game_object_manager.get_game_object("id1").value();
    REQUIRE(go1->get_name() == "go1");
    shared_ptr<Transform> transform1 = go1->get_component<Transform>().value();
    REQUIRE(transform1->position.x == 1);
}