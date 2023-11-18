#include "Instance.h"

#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <utilities/utils.h>

#include <imgui.h>
#include <backends/imgui_impl_wgpu.h>
#include <backends/imgui_impl_glfw.h>

namespace frieren_application {
    WGPUAdapter Instance::request_adapter(const WGPURequestAdapterOptions& options) {
        struct UserData {
            WGPUAdapter adapter = nullptr;
            bool requestEnded = false;
        };

        UserData userData;

        auto onAdapterRequestEnded = [] (WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData) {
            UserData& userData = *reinterpret_cast<UserData*>(pUserData);
            if (status == WGPURequestAdapterStatus_Success) {
                userData.adapter = adapter;
            } else {
                std::cout << "Could not get WebGPU adapter: " << message << std::endl;
            }
            userData.requestEnded = true;
        };

        wgpuInstanceRequestAdapter(instance, &options, onAdapterRequestEnded, (void*) &userData);

        assert(userData.requestEnded);

        return userData.adapter;
    }

    void Instance::inspect_adapter() {
        std::vector<WGPUFeatureName> features;

        size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);

        features.resize(featureCount);

        wgpuAdapterEnumerateFeatures(adapter, features.data());

        std::cout << "Adapter features:" << std::endl;
        for (auto f : features) {
            std::cout << " - " << f << std::endl;
        }

        WGPUSupportedLimits limits = {};
        limits.nextInChain = nullptr;
        bool success = wgpuAdapterGetLimits(adapter, &limits);
        if (success) {
            std::cout << "Adapter limits:" << std::endl;
            std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
            std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
            std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
            std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
            std::cout << " - maxBindGroups: " << limits.limits.maxBindGroups << std::endl;
            std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxSampledTexturesPerShaderStage: " << limits.limits.maxSampledTexturesPerShaderStage << std::endl;
            std::cout << " - maxSamplersPerShaderStage: " << limits.limits.maxSamplersPerShaderStage << std::endl;
            std::cout << " - maxStorageBuffersPerShaderStage: " << limits.limits.maxStorageBuffersPerShaderStage << std::endl;
            std::cout << " - maxStorageTexturesPerShaderStage: " << limits.limits.maxStorageTexturesPerShaderStage << std::endl;
            std::cout << " - maxUniformBuffersPerShaderStage: " << limits.limits.maxUniformBuffersPerShaderStage << std::endl;
            std::cout << " - maxUniformBufferBindingSize: " << limits.limits.maxUniformBufferBindingSize << std::endl;
            std::cout << " - maxStorageBufferBindingSize: " << limits.limits.maxStorageBufferBindingSize << std::endl;
            std::cout << " - minUniformBufferOffsetAlignment: " << limits.limits.minUniformBufferOffsetAlignment << std::endl;
            std::cout << " - minStorageBufferOffsetAlignment: " << limits.limits.minStorageBufferOffsetAlignment << std::endl;
            std::cout << " - maxVertexBuffers: " << limits.limits.maxVertexBuffers << std::endl;
            std::cout << " - maxVertexAttributes: " << limits.limits.maxVertexAttributes << std::endl;
            std::cout << " - maxVertexBufferArrayStride: " << limits.limits.maxVertexBufferArrayStride << std::endl;
            std::cout << " - maxInterStageShaderComponents: " << limits.limits.maxInterStageShaderComponents << std::endl;
            std::cout << " - maxComputeWorkgroupStorageSize: " << limits.limits.maxComputeWorkgroupStorageSize << std::endl;
            std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
            std::cout << " - maxComputeWorkgroupSizeX: " << limits.limits.maxComputeWorkgroupSizeX << std::endl;
            std::cout << " - maxComputeWorkgroupSizeY: " << limits.limits.maxComputeWorkgroupSizeY << std::endl;
            std::cout << " - maxComputeWorkgroupSizeZ: " << limits.limits.maxComputeWorkgroupSizeZ << std::endl;
            std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.limits.maxComputeWorkgroupsPerDimension << std::endl;
        }

        WGPUAdapterProperties properties = {};
        properties.nextInChain = nullptr;
        wgpuAdapterGetProperties(adapter, &properties);
        std::cout << "Adapter properties:" << std::endl;
        std::cout << " - vendorID: " << properties.vendorID << std::endl;
        std::cout << " - deviceID: " << properties.deviceID << std::endl;
        std::cout << " - name: " << properties.name << std::endl;
        if (properties.driverDescription) {
            std::cout << " - driverDescription: " << properties.driverDescription << std::endl;
        }
        std::cout << " - adapterType: " << properties.adapterType << std::endl;
        std::cout << " - backendType: " << properties.backendType << std::endl;
    }

    WGPUDevice Instance::request_device(const WGPUDeviceDescriptor& descriptor) {
        struct UserData {
            WGPUDevice device = nullptr;
            bool requestEnded = false;
        };
        UserData userData;

        auto onDeviceRequestEnded = [] (WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* pUserData) {
            UserData& userData = *reinterpret_cast<UserData*>(pUserData);
            if (status == WGPURequestDeviceStatus_Success) {
                userData.device = device;
            } else {
                std::cout << "Could not get WebGPU device: " << message << std::endl;
            }
            userData.requestEnded = true;
        };

        wgpuAdapterRequestDevice(adapter, &descriptor, onDeviceRequestEnded, (void*) &userData);

        assert(userData.requestEnded);

        return userData.device;
    }

    void Instance::inspect_device() {
        std::vector<WGPUFeatureName> features;
        size_t featureCount = wgpuDeviceEnumerateFeatures(device, nullptr);
        features.resize(featureCount);
        wgpuDeviceEnumerateFeatures(device, features.data());

        std::cout << "Device features:" << std::endl;
        for (auto f : features) {
            std::cout << " - " << f << std::endl;
        }

        WGPUSupportedLimits limits = {};
        limits.nextInChain = nullptr;
        bool success = wgpuDeviceGetLimits(device, &limits);
        if (success) {
            std::cout << "Device limits:" << std::endl;
            std::cout << " - maxTextureDimension1D: " << limits.limits.maxTextureDimension1D << std::endl;
            std::cout << " - maxTextureDimension2D: " << limits.limits.maxTextureDimension2D << std::endl;
            std::cout << " - maxTextureDimension3D: " << limits.limits.maxTextureDimension3D << std::endl;
            std::cout << " - maxTextureArrayLayers: " << limits.limits.maxTextureArrayLayers << std::endl;
            std::cout << " - maxBindGroups: " << limits.limits.maxBindGroups << std::endl;
            std::cout << " - maxDynamicUniformBuffersPerPipelineLayout: " << limits.limits.maxDynamicUniformBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxDynamicStorageBuffersPerPipelineLayout: " << limits.limits.maxDynamicStorageBuffersPerPipelineLayout << std::endl;
            std::cout << " - maxSampledTexturesPerShaderStage: " << limits.limits.maxSampledTexturesPerShaderStage << std::endl;
            std::cout << " - maxSamplersPerShaderStage: " << limits.limits.maxSamplersPerShaderStage << std::endl;
            std::cout << " - maxStorageBuffersPerShaderStage: " << limits.limits.maxStorageBuffersPerShaderStage << std::endl;
            std::cout << " - maxStorageTexturesPerShaderStage: " << limits.limits.maxStorageTexturesPerShaderStage << std::endl;
            std::cout << " - maxUniformBuffersPerShaderStage: " << limits.limits.maxUniformBuffersPerShaderStage << std::endl;
            std::cout << " - maxUniformBufferBindingSize: " << limits.limits.maxUniformBufferBindingSize << std::endl;
            std::cout << " - maxStorageBufferBindingSize: " << limits.limits.maxStorageBufferBindingSize << std::endl;
            std::cout << " - minUniformBufferOffsetAlignment: " << limits.limits.minUniformBufferOffsetAlignment << std::endl;
            std::cout << " - minStorageBufferOffsetAlignment: " << limits.limits.minStorageBufferOffsetAlignment << std::endl;
            std::cout << " - maxVertexBuffers: " << limits.limits.maxVertexBuffers << std::endl;
            std::cout << " - maxVertexAttributes: " << limits.limits.maxVertexAttributes << std::endl;
            std::cout << " - maxVertexBufferArrayStride: " << limits.limits.maxVertexBufferArrayStride << std::endl;
            std::cout << " - maxInterStageShaderComponents: " << limits.limits.maxInterStageShaderComponents << std::endl;
            std::cout << " - maxComputeWorkgroupStorageSize: " << limits.limits.maxComputeWorkgroupStorageSize << std::endl;
            std::cout << " - maxComputeInvocationsPerWorkgroup: " << limits.limits.maxComputeInvocationsPerWorkgroup << std::endl;
            std::cout << " - maxComputeWorkgroupSizeX: " << limits.limits.maxComputeWorkgroupSizeX << std::endl;
            std::cout << " - maxComputeWorkgroupSizeY: " << limits.limits.maxComputeWorkgroupSizeY << std::endl;
            std::cout << " - maxComputeWorkgroupSizeZ: " << limits.limits.maxComputeWorkgroupSizeZ << std::endl;
            std::cout << " - maxComputeWorkgroupsPerDimension: " << limits.limits.maxComputeWorkgroupsPerDimension << std::endl;
        }
    }

    void Instance::setup_material_manager() {
        this->material_manager = make_shared<MaterialManager>(
            shader_manager,
            texture_manager,
            sampler_manager
        );
        // material_manager->add_search_path("")
        material_manager->add_search_path(this->project_path / "assets");
        material_manager->load_all_descriptors();
    }

    void Instance::setup_sampler_manager() {
        this->sampler_manager = make_shared<SamplerManager>();
        sampler_manager->add_search_path(this->project_path / "assets");
        sampler_manager->init_all_sampler_descriptors();
    }

    void Instance::setup_texture_manager() {
        this->texture_manager = make_shared<TextureManager>();
        texture_manager->add_search_path(this->project_path / "assets");
        texture_manager->init_all_texture_descriptors();
    }

    void Instance::setup_shader_manager() {
        this->shader_manager = make_shared<ShaderManager>();
        shader_manager->add_search_paths(this->project_path / "assets");
        shader_manager->init_all_shaders();
    }

    void Instance::setup_mesh_manager() {
        this->mesh_manager = make_shared<MeshManager>();
        mesh_manager->init_builtin_mesh(device, queue);

        mesh_manager->add_search_path(this->project_path / "assets");
        mesh_manager->init_all_descriptors();

        mesh_manager->device = device;
        mesh_manager->queue = queue;
        // todo add search path
    }

    void Instance::create_wgpu_context() {
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
        this->window = glfwCreateWindow(640, 480, "Learn WebGPU", nullptr, nullptr);
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
        this->adapter = request_adapter(adapterOpts);

        WGPUDeviceDescriptor device_desc{};
        device_desc.nextInChain = nullptr;
        device_desc.label = "My Device";
        device_desc.requiredFeaturesCount = 0;
        device_desc.requiredLimits = nullptr;
        device_desc.defaultQueue.nextInChain = nullptr;
        device_desc.defaultQueue.label = "The default queue";
        this->device = request_device(device_desc);

        auto onDeviceError = [](WGPUErrorType type, char const *message, void *) {
            std::cout << "Uncaptured device error: type" << type;
            if (message) std::cout << " (" << message << ")";
            std::cout << std::endl;
        };
        wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError, nullptr);

        inspect_device();

        this->queue = wgpuDeviceGetQueue(device);

// #ifdef WEBGPU_BACKEND_DAWN
//         // Add a callback to monitor the moment queued work finished
//         auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void* /* pUserData */) {
//             std::cout << "Queued work finished with status: " << status << std::endl;
//         };
//         wgpuQueueOnSubmittedWorkDone(queue, 0 /* non standard argument for Dawn */, onQueueWorkDone, nullptr /* pUserData */);
// #else
        auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void *) {
            std::cout << "Queued work finished with status: " << status << std::endl;
        };
        wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr);
// #endif // WEBGPU_BACKEND_DAWN

        WGPUSwapChainDescriptor swap_chain_desc{};
        swap_chain_desc.nextInChain = nullptr;
        swap_chain_desc.width = 640;
        swap_chain_desc.height = 480;
// #ifdef WEBGPU_BACKEND_WGPU
        WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(surface, adapter);
// #else
//         WGPUTextureFormat swapChainFormat = WGPUTextureFormat_BGRA8Unorm;
// #endif
        cout << "swap chain format: " << swapChainFormat << endl;
        swap_chain_desc.format = swapChainFormat;
        swap_chain_desc.usage = WGPUTextureUsage_RenderAttachment;
        swap_chain_desc.presentMode = WGPUPresentMode_Fifo;
        this->swap_chain_desc = swap_chain_desc;

        this->swap_chain = wgpuDeviceCreateSwapChain(device, surface, &swap_chain_desc);
    }

    Instance::Instance(const filesystem::path& path) {
        // setup project path
        this->project_path = path;

        // create wgpu related context
        create_wgpu_context();

        // create builtin bind groups
        rendering_context.init(device, queue);
        this->scene_intermediate_texture = make_shared<Texture>(
            device,
            "scene_intermediate_color_texture",
            960,
            600,
            utils::get_wgpu_texture_format_pixel_size(swap_chain_desc.format),
            swap_chain_desc.format,
            WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_TextureBinding
        );
        // this->scene_internediate_depth_texture = make_shared<Texture>(Texture::create_depth_stencil_texture(
        //     device,
        //     WGPUTextureFormat
        // ))

        // setup assets managers
        this->setup_sampler_manager();
        this->setup_shader_manager();
        this->setup_texture_manager();
        this->setup_material_manager();
        this->setup_mesh_manager();

        // setup camera
        this->setup_camera();

        // setup imgui states
        create_imgui_context();
        this->setup_imgui_window();
    }

    GameObject Instance::load_game_object_from_json(const json& j) {
        GameObject go;
        from_json(j, go);

        assert(mesh_manager != nullptr);
        assert(material_manager != nullptr);
        assert(device);
        assert(queue);

        auto components_mut = go.get_components_mut();
        for (auto& component: components_mut) {
            component.second->link_referenced_mesh(*this->mesh_manager);
            component.second->link_referenced_material(device, queue, *this->material_manager);
        }

        return std::move(go);
    }

    Scene Instance::load_scene_from_json(const json& j) {
        /**
         * @brief 
         * 1. Serialize scene
         * 2. Link assets
         */

        Scene scene{};
        from_json(j, scene);

        // now scene is loaded with game objects and linked with game objects and components
        // but not linked with assets
        for (auto& item: scene.game_object_manager.get_game_objects_mut()) {
            for (auto& component: item.second->get_components_mut()) {
                component.second->link_referenced_material(device, queue, *material_manager);
                component.second->link_referenced_mesh(*mesh_manager);
            }
        }

        return std::move(scene);
    }

    Scene Instance::load_scene_from_relative_path(const filesystem::path& relative_path) {
        auto scene_abs_path = this->project_path / "assets" / relative_path;
        string json_string = utils::read_file_to_string(scene_abs_path).value();
        json j = json::parse(json_string);
        return std::move(load_scene_from_json(j));
    }

    void Instance::run() {
        while (!glfwWindowShouldClose(window)) {
            ImGuiIO& io = ImGui::GetIO();
            glfwPollEvents();

            int width, height;
            glfwGetFramebufferSize(this->window, &width, &height);

            if (width != swap_chain_desc.width || height != swap_chain_desc.height) {
//                ImGui_ImplWGPU_InvalidateDeviceObjects();
                if (swap_chain) {
                    wgpuSwapChainRelease(swap_chain);
                }
                swap_chain_desc.width = width;
                swap_chain_desc.height = height;
                swap_chain = wgpuDeviceCreateSwapChain(device, surface, &swap_chain_desc);
//                ImGui_ImplWGPU_CreateDeviceObjects();
            }

            WGPUTextureView next_texture = wgpuSwapChainGetCurrentTextureView(swap_chain);
            if (!next_texture) {
                std::cerr << "Cannot acquire next swap chain texture" << std::endl;
                break;
            }

            // setup per frame uniform
            glm::mat4x4 view_matrix = this->scene_camera->get_view_matrix();
            glm::mat4x4 projection_matrix = this->scene_camera->get_projection_matrix();
            PerFrameUniform& per_frame_uniform = rendering_context.builtin_bind_group.per_frame_uniform;
            per_frame_uniform.data.view_matrix = view_matrix;
            per_frame_uniform.data.projection_matrix = projection_matrix;
            per_frame_uniform.update_uniform_buffer(queue);

            // render scene
            this->rendering_context.set_surface_texture_view(
                this->scene_intermediate_texture->get_wgpu_texture_view(),
                // next_texture,
                960, 600,
                swap_chain_desc.format
            );
            if (this->render_pipeline != nullptr && this->current_scene != nullptr) {
                this->render_pipeline->render_scene(*this->current_scene, this->rendering_context);
            }

            // render UI
            this->rendering_context.set_surface_texture_view(next_texture, swap_chain_desc.width, swap_chain_desc.height, this->swap_chain_desc.format);
            ImGui_ImplWGPU_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGuiWindowFlags root_window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar
                | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            static bool p_open = true;
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window
            ImGui::Begin("Dockspace example", &p_open, root_window_flags);
            ImGui::PopStyleVar(2);

            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockSpace(dockspace_id);

            vector<shared_ptr<GameObject>> game_objects;
            for (const auto& go: current_scene->game_object_manager.get_game_objects()) {
                game_objects.push_back(go.second);
            }
            ImGui::SetNextWindowDockID(dockspace_id);
            this->imgui_root.hierarchy_window.draw(game_objects);
            auto go = current_scene->game_object_manager.get_game_objects().begin()->second;
            this->imgui_root.inspector_window.set_current_game_object(go);
            this->imgui_root.inspector_window.draw();
            this->imgui_root.scene_window.draw(scene_intermediate_texture->get_wgpu_texture_view(), 960, 600);
            this->imgui_root.stats_window.draw();

            // ImGui::Begin("Scene");
            // ImGui::Image(scene_intermediate_texture->get_wgpu_texture_view(), ImVec2{
            //     960,
            //     600
            // });
            // ImGui::End();

            ImGui::End();

            ImGui::Render();
            this->rendering_context.draw_imgui();

            wgpuTextureViewRelease(next_texture);
            wgpuSwapChainPresent(swap_chain);
        }
    }

    void Instance::set_current_scene(shared_ptr<Scene> scene) {
        this->current_scene = std::move(scene);
    }

    void Instance::set_render_pipeline(shared_ptr<RenderPipeline> pipeline) {
        this->render_pipeline = pipeline;
    }

    void Instance::create_imgui_context() {
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

    void Instance::setup_camera() {
        this->scene_camera = make_shared<PerspectiveCamera>();
        float aspect = (float) 960 / 600;
        // float aspect = (float) 600 / 960;
        this->scene_camera->set_perspective_parameters(3.14f / 4, aspect, 0.1, 200);

        this->scene_camera_controller.camera = this->scene_camera;
        scene_camera_controller.translate_speed = 10;
    }

    void Instance::setup_imgui_window() {
        imgui_root.scene_window.camera_controller = &this->scene_camera_controller;

        imgui_root.hierarchy_window.on_select_game_object = [&] (const string& new_id) {
            this->imgui_root.hierarchy_window.selected_game_object_id = new_id;
        };
    }
}
