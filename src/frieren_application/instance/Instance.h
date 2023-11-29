#ifndef FRIEREN_CORE_INSTANCE_H
#define FRIEREN_CORE_INSTANCE_H

#include <webgpu/webgpu.hpp>
#include <material/MaterialManager.h>
#include <shader/ShaderManager.h>
#include <texture/SamplerManager.h>
#include <texture/TextureManager.h>
#include <common_include.h>
#include <mesh/MeshManager.h>
#include <scene/GameObject.h>
#include <scene/Scene.h>
#include <texture/Texture.h>
#include <mesh/Mesh.h>
#include <common_include_glm.h>
#include <rendering/BuiltinBindGroupLayout.h>
#include <rendering/RenderingContext.h>
#include <pipeline/RenderPipeline.h>
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <inspector/InspectorWindow.h>
#include <ImGuiRoot.h>
#include <camera/PerspectiveCamera.h>
#include <texture/Texture.h>

using namespace std;
using namespace frieren_core;
using namespace frieren_editor;

namespace frieren_application {
    class Instance {
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

        filesystem::path project_path;
    
        shared_ptr<MaterialManager> material_manager;
        shared_ptr<ShaderManager> shader_manager;
        shared_ptr<SamplerManager> sampler_manager;
        shared_ptr<TextureManager> texture_manager;
        shared_ptr<MeshManager> mesh_manager;

        // rendering context
        RenderingContext rendering_context;
        // pipeline
        shared_ptr<RenderPipeline> render_pipeline;
        // scene
        shared_ptr<Scene> current_scene;
        // intermediate image for rendering the scene
        shared_ptr<Texture> scene_intermediate_texture;
        // shared_ptr<Texture> scene_internediate_depth_texture;

        WGPUAdapter request_adapter(const WGPURequestAdapterOptions& options);

        // imgui states
        ImGuiRoot imgui_root{};
        shared_ptr<GameObject> selected_game_object;

        // scene camera state
        shared_ptr<PerspectiveCamera> scene_camera;
        // PerspectiveCamera scene_camera;
        PerspectiveCameraController scene_camera_controller;
    
        void inspect_adapter();

        WGPUDevice request_device(const WGPUDeviceDescriptor& desc);

        void inspect_device();

        void create_wgpu_context();

        void create_imgui_context();

        void setup_material_manager();

        void setup_shader_manager();

        void setup_sampler_manager();

        void setup_texture_manager();

        void setup_mesh_manager();

        void setup_camera();

        void setup_imgui_window();

        void build_dock_node();
    public:
        // Instance();
        explicit Instance(const filesystem::path& path);

        /**
         * @brief load game object from json except for component-wise references
         * the references for assets (including material, shader, sampler, texture, mesh)
         * will be set, but the references for Component anf GameObject will not be set
         * @param j 
         * @return GameObject 
         */
        GameObject load_game_object_from_json(const json& j);

        Scene load_scene_from_json(const json& j);

        /**
         * @brief load scene from asset path
         * a scene located at /project_root/assets/scenes/s1.json can be loaded by "scenes/s1.json"
         * @param relative_path 
         * @return Scene 
         */
        Scene load_scene_from_relative_path(const filesystem::path& relative_path);

        void set_current_scene(shared_ptr<Scene> scene);

        void set_render_pipeline(shared_ptr<RenderPipeline> pipeline);

        void run();
    };
}

#endif
