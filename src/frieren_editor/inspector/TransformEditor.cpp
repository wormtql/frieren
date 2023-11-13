#include "TransformEditor.h"
#include <imgui.h>

namespace frieren_editor {
    TransformEditor::TransformEditor(shared_ptr<Transform> transform)
        : ComponentEditor{transform} 
    {
        this->position = transform->position;
        this->rotation = transform->rotation;
        this->scale = transform->scale;
    }

    void TransformEditor::draw() {
        ImGui::Text("Position");
        ImGui::InputFloat3("Position", reinterpret_cast<float*>(&position));

        ImGui::InputFloat3("Rotation", &rotation.r);

        ImGui::InputFloat("Scale", &scale);
    }
}
