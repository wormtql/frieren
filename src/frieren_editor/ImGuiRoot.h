#ifndef FRIEREN_CORE_IMGUIROOT_H
#define FRIEREN_CORE_IMGUIROOT_H

#include <imgui.h>
#include <backends/imgui_impl_wgpu.h>
#include <backends/imgui_impl_glfw.h>
#include "hierarchy/HierarchyWindow.h"
#include "inspector/InspectorWindow.h"

namespace frieren_editor {
    class ImGuiRoot {
    public:
        HierarchyWindow hierarchy_window{};
        InspectorWindow inspector_window{};
        // void draw()
    };
}

#endif //FRIEREN_IMGUIROOT_H
