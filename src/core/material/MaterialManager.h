#ifndef FRIEREN_CORE_MATERIAL_MANAGER_H
#define FRIEREN_CORE_MATERIAL_MANAGER_H

#include <common_include.h>
#include "Material.h"
#include <shader/ShaderManager.h>
#include <texture/TextureManager.h>
#include <texture/SamplerManager.h>

using namespace std;

namespace frieren_core {
    class MaterialManager {
    private:
        vector<filesystem::path> search_paths;

        // id -> desc
        map<string, MaterialDescriptor> loaded_descriptors;
        // id -> mat
        map<string, shared_ptr<Material>> loaded_materials;

        shared_ptr<ShaderManager> shader_manager;
        shared_ptr<TextureManager> texture_manager;
        shared_ptr<SamplerManager> sampler_manager;

        void load_material_descriptor(const filesystem::path& path);
    public:
        MaterialManager(shared_ptr<ShaderManager> shader_m, shared_ptr<TextureManager> texture_m, shared_ptr<SamplerManager> sampler_m);
        MaterialManager(const MaterialManager& other) = delete;

        void add_search_path(const filesystem::path& path);

        void load_all_descriptors();

        optional<shared_ptr<Material>> get_material(WGPUDevice device, WGPUQueue queue, const string& id);
    };
}

#endif
