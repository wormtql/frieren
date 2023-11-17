#include "SceneWindow.h"
#include <imgui.h>

namespace frieren_editor {
    void SceneWindow::draw(WGPUTextureView texture_view, int width, int height) {
        ImGuiIO& io = ImGui::GetIO();

        ImGui::Begin("Scene", &show_window);

        ImGui::Image(texture_view, ImVec2{
            (float)width,
            (float)height
        });

        if (ImGui::IsWindowHovered()) {
            if (ImGui::IsMousePosValid()) {
                float mouse_delta_x = io.MouseDelta.x;
                float mouse_delta_y = io.MouseDelta.y;

                if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                    float dx = 0;
                    float dy = 0;
                    if (ImGui::IsKeyDown(ImGuiKey_A)) {
                        dx -= io.DeltaTime;
                    }
                    if (ImGui::IsKeyDown(ImGuiKey_D)) {
                        dx += io.DeltaTime;
                    }
                    if (ImGui::IsKeyDown(ImGuiKey_S)) {
                        dy -= io.DeltaTime;
                    }
                    if (ImGui::IsKeyDown(ImGuiKey_W)) {
                        dy += io.DeltaTime;
                    }

                    if (dx != 0 || dy != 0) {
                        // cout << dx << ", " << dy << endl;
                        this->camera_controller->move_camera_local(dx, dy);
                    }
                }
            }
        }

        ImGui::End();
    }
}