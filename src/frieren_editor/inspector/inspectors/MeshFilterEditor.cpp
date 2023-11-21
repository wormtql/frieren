#include "MeshFilterEditor.h"
#include <imgui.h>

namespace frieren_editor {
    MeshFilterEditor::MeshFilterEditor(shared_ptr<MeshFilter> component): ComponentEditor(component) {
        this->mesh_name = component->get_mesh()->name;
        this->mesh_id = component->get_mesh()->id;
    }

    void MeshFilterEditor::draw() {
        ImGui::Text(this->mesh_name.c_str());
        ImGui::Text(this->mesh_id.c_str());
    }
}