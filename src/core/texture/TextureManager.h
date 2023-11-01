#ifndef FRIEREN_CORE_TEXTURE_MANAGER
#define FRIEREN_CORE_TEXTURE_MANAGER

#include <common_include.h>
#include "Texture.h"

using namespace std;

namespace frieren_core {
    class TextureManager {
    private:
        vector<filesystem::path> search_paths;

        map<string, TextureDescriptor> loaded_texture_descriptors;
        map<string, shared_ptr<Texture>> loaded_textures;

        void load_texture_descriptor(const filesystem::path& path);
    public:
        TextureManager() = default;
        TextureManager(const TextureManager& other) = delete;
        
        void add_search_path(const filesystem::path& p);

        void init_all_texture_descriptors();

        optional<shared_ptr<Texture>> get_texture(WGPUDevice device, WGPUQueue queue, const string& name);
    };
}

#endif
