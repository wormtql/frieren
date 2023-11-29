#include "SimpleInstance.h"

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <utilities/utils.h>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_wgpu.h>
#include <backends/imgui_impl_glfw.h>
#include <common/wgpu_common_function.h>

namespace frieren_application {
    void SimpleInstance::create_wgpu_context() {
        WGPUInstanceDescriptor instance_desc{};
        instance_desc.nextInChain = nullptr;

        this->instance = wgpuCreateInstance(&instance_desc);
        if (!instance) {
            std::cerr << "Could not initialize WebGPU!" << std::endl;
            assert(false);
        }
        std::cout << "WGPU instance: " << instance << std::endl;

        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        this->window = glfwCreateWindow(640, 480, "WebGPU Window", nullptr, nullptr);
        this->window_width = 640;
        this->window_height = 480;
        if (!window) {
            std::cerr << "Could not open window!" << std::endl;
            glfwTerminate();
            assert(false);
        }

        this->surface = glfwGetWGPUSurface(instance, window);

        WGPURequestAdapterOptions adapterOpts{};
        adapterOpts.nextInChain = nullptr;
        adapterOpts.compatibleSurface = surface;
        this->adapter = WGPUCommonFunction::request_adapter(adapterOpts, instance);

        WGPUDeviceDescriptor device_desc{};
        device_desc.nextInChain = nullptr;
        device_desc.label = "My Device";
        device_desc.requiredFeaturesCount = 0;
        device_desc.requiredLimits = nullptr;
        device_desc.defaultQueue.nextInChain = nullptr;
        device_desc.defaultQueue.label = "The default queue";
        this->device = WGPUCommonFunction::request_device(device_desc, adapter);

        auto onDeviceError = [](WGPUErrorType type, char const *message, void *) {
            std::cout << "Uncaptured device error: type" << type;
            if (message) std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError, nullptr);

        WGPUCommonFunction::inspect_device(device);

        this->queue = wgpuDeviceGetQueue(device);

        auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void *) {
            std::cout << "Queued work finished with status: " << status << std::endl;
        };
        wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr);

        WGPUSwapChainDescriptor swap_chain_desc{};
        swap_chain_desc.nextInChain = nullptr;
        swap_chain_desc.width = 640;
        swap_chain_desc.height = 480;
        WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(surface, adapter);

        cout << "swap chain format: " << swapChainFormat << endl;
        swap_chain_desc.format = swapChainFormat;
        swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
        swap_chain_desc.presentMode = WGPUPresentMode_Fifo;
        this->swap_chain_desc = swap_chain_desc;

        this->swap_chain = wgpuDeviceCreateSwapChain(device, surface, &swap_chain_desc);
    }

    SimpleInstance::SimpleInstance() {
        // create wgpu related context
        create_wgpu_context();

        // setup imgui states
        create_imgui_context();
    }

    void SimpleInstance::run() {
        while (!glfwWindowShouldClose(window)) {
            ImGuiIO& io = ImGui::GetIO();
            glfwPollEvents();

            int width, height;
            glfwGetFramebufferSize(this->window, &width, &height);

            if (width != swap_chain_desc.width || height != swap_chain_desc.height) {
                ImGui_ImplWGPU_InvalidateDeviceObjects();
                if (swap_chain) {
                    wgpuSwapChainRelease(swap_chain);
                }
                cout << "(" << width << ", " << height << ")" << endl;
                swap_chain_desc.width = width;
                swap_chain_desc.height = height;
                swap_chain = wgpuDeviceCreateSwapChain(device, surface, &swap_chain_desc);
                ImGui_ImplWGPU_CreateDeviceObjects();
            }

            WGPUTextureView next_texture = wgpuSwapChainGetCurrentTextureView(swap_chain);
            if (!next_texture) {
                std::cerr << "Cannot acquire next swap chain texture" << std::endl;
                continue;
            }

            // render UI
            ImGui_ImplWGPU_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("SampleWindow");
            ImGui::Text("123");
            ImGui::End();
            ImGui::Render();
            draw_imgui(next_texture);

            wgpuTextureViewRelease(next_texture);
            wgpuSwapChainPresent(swap_chain);
        }
    }

    void SimpleInstance::draw_imgui(WGPUTextureView texture_view) {
        WGPURenderPassColorAttachment color_attachments{};
        color_attachments.loadOp = WGPULoadOp_Load;
        color_attachments.storeOp = WGPUStoreOp_Store;
        color_attachments.view = texture_view;
        WGPURenderPassDescriptor render_pass_desc{};
        render_pass_desc.colorAttachmentCount = 1;
        render_pass_desc.colorAttachments = &color_attachments;
        render_pass_desc.depthStencilAttachment = nullptr;
        render_pass_desc.label = "Draw ImGui Render Pass";

        WGPUCommandEncoderDescriptor enc_desc{};
        enc_desc.label = "Draw ImGui";
        WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(this->device, &enc_desc);

        WGPURenderPassEncoder pass = wgpuCommandEncoderBeginRenderPass(encoder, &render_pass_desc);
        ImGui_ImplWGPU_RenderDrawData(ImGui::GetDrawData(), pass);
        wgpuRenderPassEncoderEnd(pass);

        WGPUCommandBufferDescriptor cmd_buffer_desc{};
        WGPUCommandBuffer cmd_buffer = wgpuCommandEncoderFinish(encoder, &cmd_buffer_desc);

        wgpuQueueSubmit(this->queue, 1, &cmd_buffer);

        wgpuCommandBufferRelease(cmd_buffer);
        wgpuRenderPassEncoderRelease(pass);
        wgpuCommandEncoderRelease(encoder);
    }

    void SimpleInstance::create_imgui_context() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        io.IniFilename = nullptr;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOther(this->window, true);
        ImGui_ImplWGPU_Init(this->device, 3, this->swap_chain_desc.format, WGPUTextureFormat_Undefined);
    }
}
