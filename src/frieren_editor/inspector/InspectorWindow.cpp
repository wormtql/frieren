#include "InspectorWindow.h"
#include <imgui.h>

namespace frieren_editor {
    InspectorWindow::InspectorWindow() {
        inspector_registry.init();
    }

    void InspectorWindow::set_current_game_object(shared_ptr<GameObject> game_object) {
        this->current_game_object = game_object;
        this->editors.clear();

        for (const auto& comp: game_object->get_components()) {
            auto inspector = inspector_registry.create_inspector(comp.first, comp.second);
            if (inspector) {
                this->editors.push_back(inspector);
            }
        }
    }

    void InspectorWindow::draw() {
        if (ImGui::Begin("Inspector", &show_window)) {
            for (const auto& inspector: this->editors) {
                inspector->draw();
            }
            ImGui::Text("%d", show_window);
            ImGui::End();
        }
    }
}
