#include "SamplerManager.h"
#include <utilities/utils.h>

namespace frieren_core {
    void SamplerManager::add_search_path(const filesystem::path& path) {
        search_paths.push_back(path);
    }

    void SamplerManager::load_sampler_descriptor(const filesystem::path& path) {
        if (filesystem::exists(path)) {
            string sampler_desc_json = utils::read_file_to_string(path).value();
            json j = json::parse(sampler_desc_json);

            SamplerDescriptor desc = j.template get<SamplerDescriptor>();
            loaded_sampler_descriptors[desc.name] = desc;
        }
    }

    void SamplerManager::init_all_sampler_descriptors() {
        for (const auto& search_path: this->search_paths) {
            for (const auto& dir_entry: filesystem::recursive_directory_iterator(search_path)) {
                if (dir_entry.is_regular_file()) {
                    filesystem::path p = dir_entry.path();
                    filesystem::path filename = p.filename();
                    auto name = filename.string();
                    if (name.ends_with(".sampler.json")) {
                        load_sampler_descriptor(p);
                    }
                }
            }
        }
    }

    optional<shared_ptr<Sampler>> SamplerManager::get_sampler(WGPUDevice device, const string& name) {
        if (loaded_samplers.find(name) != loaded_samplers.end()) {
            return loaded_samplers[name];
        }

        if (loaded_sampler_descriptors.find(name) == loaded_sampler_descriptors.end()) {
            return {};
        }

        const auto& desc = loaded_sampler_descriptors[name];
        shared_ptr<Sampler> sampler = make_shared<Sampler>(device, desc);
        loaded_samplers[name] = sampler;

        return sampler;
    }
}