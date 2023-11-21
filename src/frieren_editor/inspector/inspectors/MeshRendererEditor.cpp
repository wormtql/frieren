#include "MeshRendererEditor.h"
#include <imgui.h>

namespace frieren_editor {
    MeshRendererEditor::MeshRendererEditor(shared_ptr<MeshRenderer> component)
        : ComponentEditor(component)
    {
        for (const auto& mat: component->get_materials()) {
            this->material_editors.emplace_back(mat.object);
        }
    }

    void MeshRendererEditor::draw() {
        for (int i = 0; i < material_editors.size(); i++) {
            ImGui::Text("Material %d", i);
            material_editors[i].draw();
        }
    }
}