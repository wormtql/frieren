#ifndef FRIEREN_EDITOR_SCENE_WINDOW_H
#define FRIEREN_EDITOR_SCENE_WINDOW_H

#include <webgpu/webgpu.hpp>
#include <camera/PerspectiveCamera.h>
#include <common_include.h>

using namespace std;
using namespace frieren_core;

namespace frieren_editor {
    class SceneWindow {
    public:
        bool show_window = true;
        PerspectiveCameraController* camera_controller = nullptr;

        SceneWindow() = default;

        void draw(WGPUTextureView texture_view, int width, int height);
    };
}

#endif
