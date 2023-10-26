#include <shader/Shader.h>
#include <shader/ShaderManager.h>
#include <common_include.h>

using namespace std;
using namespace frieren_core;

int main() {
    ShaderManager shader_manager{};
    shader_manager.add_search_paths(R"(E:\CLionProjects\frieren\src\shaders)");
    shader_manager.init_all_shaders();

    // ifstream shader_source(R"(E:\CLionProjects\frieren\src\shaders\druvis.color.json)");
    // json data = json::parse(shader_source);
    // cout << data << endl;

    // auto desc = data.template get<frieren_core::ShaderDescriptor>();
    // cout << desc.name;

    return 0;
}