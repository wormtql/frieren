#include "HierarchyWindow.h"
#include <imgui.h>

using frieren_core::component::Transform;

namespace frieren_editor {
    void HierarchyWindow::draw_node(shared_ptr<GameObject> go) {
        shared_ptr<Transform> transform = go->get_component<Transform>().value();
        const string& name = go->get_name();
        const string& id = go->get_id();

        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_SpanFullWidth;

        if (selected_game_object_id.has_value() && selected_game_object_id.value() == id) {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }

        if (!transform->has_children()) {
            node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
            ImGui::TreeNodeEx(name.c_str(), node_flags);
            // ImGui::Text(name.c_str());
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                if (this->on_select_game_object.has_value()) {
                    on_select_game_object.value()(id);
                }
            }
        } else {
            bool node_open = ImGui::TreeNodeEx(name.c_str(), node_flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                if (on_select_game_object.has_value()) {
                    on_select_game_object.value()(id);
                }
            }
            if (node_open) {
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
        

        ImGui::End();
    }
}