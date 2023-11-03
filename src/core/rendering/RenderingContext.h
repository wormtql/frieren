#ifndef FRIEREN_CORE_RENDERING_CONTEXT_H
#define FRIEREN_CORE_RENDERING_CONTEXT_H

#include <webgpu/webgpu.hpp>
#include <material/MaterialManager.h>
#include <shader/ShaderManager.h>
#include <texture/SamplerManager.h>
#include <texture/TextureManager.h>
#include <common_include.h>
#include <mesh/MeshManager.h>

using namespace std;

namespace frieren_core {
    class RenderingContext {
    private:
        WGPUInstance instance;
        WGPUSurface surface;
        WGPUAdapter adapter;
        WGPUDevice device;
        WGPUQueue queue;
        WGPUSwapChain swap_chain;
    
        shared_ptr<MaterialManager> material_manager;
        shared_ptr<ShaderManager> shader_manager;
        shared_ptr<SamplerManager> sampler_manager;
        shared_ptr<TextureManager> texture_manager;
        shared_ptr<MeshManager> mesh_manager;

        WGPUAdapter request_adapter(const WGPURequestAdapterOptions& options);
    
        void inspect_adapter();

        WGPUDevice request_device(const WGPUDeviceDescriptor& desc);

        void inspect_device();

        void setup_material_manager();

        void setup_shader_manager();

        void setup_sampler_manager();

        void setup_texture_manager();

        void setup_mesh_manager();
    public:
        RenderingContext();


    };
}

#endif
