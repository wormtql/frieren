#ifndef FRIEREN_SIMPLEINSTANCE_H
#define FRIEREN_SIMPLEINSTANCE_H


#include <webgpu/webgpu.hpp>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>

using namespace std;

namespace frieren_application {
    class SimpleInstance {
    private:
        WGPUInstance instance;
        WGPUSurface surface;
        WGPUAdapter adapter;
        WGPUDevice device;
        WGPUQueue queue;
        WGPUSwapChain swap_chain;
        WGPUSwapChainDescriptor swap_chain_desc{};
        GLFWwindow* window;
        int window_width;
        int window_height;

        void create_wgpu_context();

        void create_imgui_context();

        void draw_imgui(WGPUTextureView texture_view);
    public:
        explicit SimpleInstance();

        void run();
    };
}

#endif //FRIEREN_SIMPLEINSTANCE_H
