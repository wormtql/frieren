//
// Created by 58413 on 2023/10/17.
//

#include <memory>
#include <utilities/utils.h>
#include <nlohmann/json.hpp>
#include "ShaderManager.h"

using json = nlohmann::json;

namespace frieren_core {
    void ShaderManager::add_search_paths(filesystem::path p) {
        search_paths.push_back(p);
    }

    void ShaderManager::init_all_shaders() {
        for (const auto& search_path: this->search_paths) {
            for (const auto& dir_entry: filesystem::recursive_directory_iterator(search_path)) {
                if (dir_entry.is_regular_file()) {
                    filesystem::path p = dir_entry.path();
                    filesystem::path filename = p.filename();
                    auto name = filename.string();
                    if (name.ends_with(".shader.json")) {
                        load_shader_descriptor(p);
                    }
                }
            }
        }
    }

    bool ShaderManager::load_shader_descriptor(const filesystem::path& path) {
        if (filesystem::exists(path)) {
            string shader_meta_string = utils::read_file_to_string(path).value();
            json j = json::parse(shader_meta_string);
            ShaderDescriptor shader_desc = j.template get<ShaderDescriptor>();

            filesystem::path shader_source_path = path;
            shader_source_path += shader_desc.shader_source_path;
            if (filesystem::exists(shader_source_path)) {
                string shader_source_string = utils::read_file_to_string(shader_source_path).value();
                shader_desc.shader_source = move(shader_source_string);
            }

            if (!loaded_shader_descriptors.contains(shader_desc.name)) {
                loaded_shader_descriptors[shader_desc.name] = shader_desc;
            }

            return true;
        }

        return false;
    }

    optional<shared_ptr<Shader>> ShaderManager::get_shader(WGPUDevice device, const string& name) {
        if (loaded_shaders.find(name) != loaded_shaders.end()) {
            return loaded_shaders[name];
        }

        if (!loaded_shader_descriptors.contains(name)) {
            return {};
        } else {
            const auto& shader_desc = loaded_shader_descriptors[name];
            // Shader shader{device, shader_desc};
            auto shader = make_shared<Shader>(device, shader_desc);
            loaded_shaders[name] = shader;
            return shader;
        }
    }
}
