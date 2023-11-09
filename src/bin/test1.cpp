#include <shader/Shader.h>
#include <shader/ShaderManager.h>
#include <common_include.h>
#include <components/Component.h>
#include <components/Transform.h>
#include <components/ComponentRegistry.h>
#include <rendering/Instance.h>

using namespace std;
using namespace frieren_core;
using namespace frieren_core::component;

int main() {
    Instance instance{R"(E:\CLionProjects\frieren\test_project)"};
    Scene scene = instance.load_scene_from_relative_path("scenes/sample_scene.json");
    cout << scene.id;

    return 0;
}