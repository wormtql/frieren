//
// Created by 58413 on 2023/10/17.
//

#ifndef FRIEREN_SHADERMANAGER_H
#define FRIEREN_SHADERMANAGER_H

#include <common_include.h>
#include "Shader.h"

using namespace std;

namespace frieren_core {
    class ShaderManager {
    private:
        vector<filesystem::path> search_paths;

        map<string, shared_ptr<Shader>> loaded_shaders;
        map<string, ShaderDescriptor> loaded_shader_descriptors;

        bool load_shader_descriptor(const filesystem::path& path);
    public:
        ShaderManager() = default;

        void init_all_shaders();

        void add_search_paths(filesystem::path p);

        optional<shared_ptr<Shader>> get_shader(WGPUDevice device, const string& name);
    };
}



#endif //FRIEREN_SHADERMANAGER_H
