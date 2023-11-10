#include <shader/Shader.h>
#include <shader/ShaderManager.h>
#include <common_include.h>
#include <components/Component.h>
#include <components/Transform.h>
#include <components/ComponentRegistry.h>
#include <rendering/Instance.h>
#include <pipeline/DisplayColorRenderPipeline.h>

using namespace std;
using namespace frieren_core;
using namespace frieren_core::component;

int main() {
    Instance instance{R"(E:\CLionProjects\frieren\test_project)"};
    shared_ptr<Scene> scene = make_shared<Scene>(std::move(instance.load_scene_from_relative_path("scenes/sample_scene.json")));
    cout << scene->id;
    instance.set_current_scene(scene);

    shared_ptr<DisplayColorRenderPipeline> display_color_render_pipeline = make_shared<DisplayColorRenderPipeline>();
    instance.set_render_pipeline(std::reinterpret_pointer_cast<RenderPipeline>(display_color_render_pipeline));

    instance.run();

    return 0;
}