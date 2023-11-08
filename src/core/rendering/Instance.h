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
#include "BuiltinBindGroupLayout.h"
#include "RenderingContext.h"

using namespace std;

namespace frieren_core {
    class Instance {
    private:
        WGPUInstance instance;
        WGPUSurface surface;
        WGPUAdapter adapter;
        WGPUDevice device;
        WGPUQueue queue;
        WGPUSwapChain swap_chain;

        filesystem::path project_path;
    
        shared_ptr<MaterialManager> material_manager;
        shared_ptr<ShaderManager> shader_manager;
        shared_ptr<SamplerManager> sampler_manager;
        shared_ptr<TextureManager> texture_manager;
        shared_ptr<MeshManager> mesh_manager;

        // rendering context
        RenderingContext rendering_context;

        WGPUAdapter request_adapter(const WGPURequestAdapterOptions& options);
    
        void inspect_adapter();

        WGPUDevice request_device(const WGPUDeviceDescriptor& desc);

        void inspect_device();

        void create_wgpu_context();

        void setup_material_manager();

        void setup_shader_manager();

        void setup_sampler_manager();

        void setup_texture_manager();

        void setup_mesh_manager();
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
    };
}

#endif
