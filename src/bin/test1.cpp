#include <shader/Shader.h>
#include <shader/ShaderManager.h>
#include <common_include.h>
#include <components/Component.h>
#include <components/Transform.h>
#include <components/ComponentRegistry.h>

using namespace std;
using namespace frieren_core;
using namespace frieren_core::component;

int main() {
    ComponentRegistry component_reg;
    cout << component_reg.creator_registry.size() << endl;

    return 0;
}