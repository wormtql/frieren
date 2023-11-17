#include "HierarchyWindow.h"
#include <imgui.h>

using frieren_core::component::Transform;

namespace frieren_editor {
    void HierarchyWindow::draw_node(shared_ptr<GameObject> go) {
        shared_ptr<Transform> transform = go->get_component<Transform>().value();
        const string& name = go->get_name();
        if (!transform->has_children()) {
            ImGui::Text(name.c_str());
        } else {
            if (ImGui::TreeNode(name.c_str())) {
                for (const auto& c: transform->get_children()) {
                    draw_node(c.object->get_game_object().value());
                }
                ImGui::TreePop();
            }
        }
    }

    void HierarchyWindow::draw(const vector<shared_ptr<GameObject>> &objects) {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Hierarchy", &show_window);

        vector<shared_ptr<GameObject>> all_roots;

        for (int i = 0; i < objects.size(); i++) {
            if (!objects[i]->has_parent()) {
                all_roots.push_back(objects[i]);
            }
        }

        for (auto go: all_roots) {
            draw_node(go);
        }

        if (ImGui::IsWindowHovered()) {
            if (ImGui::IsMousePosValid()) {
                ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
            }
            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDown(i)) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        }
        // ImGui::Hover
        // ImGui::IsMouseDown(ImGuiMouseButton_Rig)
        // ImGui::Image()
        

        ImGui::End();
    }
}