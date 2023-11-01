#ifndef FRIEREN_CORE_SAMPLER_MANAGER_H
#define FRIEREN_CORE_SAMPLER_MANAGER_H

#include <common_include.h>
#include "Sampler.h"
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    class SamplerManager {
    private:
        vector<filesystem::path> search_paths;

        map<string, shared_ptr<Sampler>> loaded_samplers;
        map<string, SamplerDescriptor> loaded_sampler_descriptors;

        void load_sampler_descriptor(const filesystem::path& path);
    public:
        SamplerManager() = default;
        SamplerManager(const SamplerManager& other) = delete;
        
        void add_search_path(const filesystem::path& path);

        void init_all_sampler_descriptors();

        optional<shared_ptr<Sampler>> get_sampler(WGPUDevice device, const string& name);
    };
}

#endif
