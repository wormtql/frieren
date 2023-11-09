#include "MaterialManager.h"
#include <utilities/utils.h>


namespace frieren_core {
    MaterialManager::MaterialManager(
        shared_ptr<ShaderManager> shader_m,
        shared_ptr<TextureManager> texture_m,
        shared_ptr<SamplerManager> sampler_m
    ): shader_manager(shader_m), texture_manager(texture_m), sampler_manager(sampler_m) {

    }

    void MaterialManager::add_search_path(const filesystem::path& path) {
        search_paths.push_back(path);
    }

    void MaterialManager::load_material_descriptor(const filesystem::path& path) {
        if (filesystem::exists(path)) {
            string material_desc_json = utils::read_file_to_string(path).value();
            json j = json::parse(material_desc_json);

            MaterialDescriptor desc = j.template get<MaterialDescriptor>();
            loaded_descriptors[desc.id.value()] = desc;
        }
    }

    void MaterialManager::load_all_descriptors() {
        for (const auto& search_path: this->search_paths) {
            for (const auto& dir_entry: filesystem::recursive_directory_iterator(search_path)) {
                if (dir_entry.is_regular_file()) {
                    filesystem::path p = dir_entry.path();
                    filesystem::path filename = p.filename();
                    auto name = filename.string();
                    if (name.ends_with(".material.json")) {
                        load_material_descriptor(p);
                    }
                }
            }
        }
    }

    optional<shared_ptr<Material>> MaterialManager::get_material(WGPUDevice device, WGPUQueue queue, const string& id) {
        if (loaded_materials.find(id) != loaded_materials.end()) {
            return loaded_materials[id];
        }

        if (loaded_descriptors.find(id) == loaded_descriptors.end()) {
            return {};
        }

        const auto& desc = loaded_descriptors[id];
        shared_ptr<Material> mat = make_shared<Material>(
            device,
            queue,
            desc,
            *texture_manager.get(),
            *sampler_manager.get(),
            *shader_manager.get()
        );

        loaded_materials[id] = mat;
        return mat;
    }
}
