#include "StatsWindow.h"
#include <imgui.h>

namespace frieren_editor {
    void StatsWindow::draw() {
        ImGuiIO& io = ImGui::GetIO();

        float delta_time = io.DeltaTime;

        ImGui::Begin("Stats", &window_open);

        ImGui::Text("frame rate: %.2f", 1.0f / delta_time);

        ImGui::End();
    }
}