#include "InspectorWindow.h"
#include <imgui.h>

namespace frieren_editor {
    InspectorWindow::InspectorWindow() {
        inspector_registry.init();
    }

    void InspectorWindow::set_current_game_object(shared_ptr<GameObject> game_object) {
        this->current_game_object = game_object;
        this->editors.clear();
        this->component_names.clear();

        for (const auto& comp: game_object->get_components()) {
            auto inspector = inspector_registry.create_inspector(comp.first, comp.second);
            if (inspector) {
                this->editors.push_back(inspector);
                this->component_names.push_back(comp.first);
            }
        }
    }

    void InspectorWindow::draw() {
        if (ImGui::Begin("InspectorWindow", &show_window)) {
            for (int i = 0; i < editors.size(); i++) {
                const string& name = component_names[i];
                if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_None)) {
                    editors[i]->draw();
                }
            }
        }
        ImGui::End();
    }
}
