#ifndef FRIEREN_EDITOR_INSPECTOR_WINDOW
#define FRIEREN_EDITOR_INSPECTOR_WINDOW

#include <memory>
#include <vector>
#include <string>

#include <scene/GameObject.h>
#include "ComponentEditor.h"
#include "all_inspectors.h"
#include "InspectorRegistry.h"

using namespace std;
using namespace frieren_core;

namespace frieren_editor {
    class InspectorWindow {
    private:
        shared_ptr<GameObject> current_game_object;
        vector<shared_ptr<ComponentEditor>> editors;
        vector<string> component_names;

        InspectorRegistry inspector_registry;
    public:
        bool show_window = true;

        InspectorWindow();

        void draw();

        void set_current_game_object(shared_ptr<GameObject> game_object);
    };
}

#endif