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
        shared_ptr<Transform> component = dynamic_pointer_cast<Transform>(this->related_component);
        ImGui::Text("Position");
        if (ImGui::InputFloat3("Position", reinterpret_cast<float*>(&position))) {
            component->position = this->position;
        }

        if (ImGui::InputFloat3("Rotation", &rotation.r)) {
            component->rotation = this->rotation;
        }

        if (ImGui::InputFloat("Scale", &scale)) {
            component->scale = this->scale;
        }
    }
}
