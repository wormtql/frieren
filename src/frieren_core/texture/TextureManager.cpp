#include "TextureManager.h"
#include <utilities/utils.h>

namespace frieren_core {
    void TextureManager::add_search_path(const filesystem::path& p) {
        search_paths.push_back(p);
    }

    void TextureManager::init_all_texture_descriptors() {
        for (const auto& search_path: this->search_paths) {
            for (const auto& dir_entry: filesystem::recursive_directory_iterator(search_path)) {
                if (dir_entry.is_regular_file()) {
                    filesystem::path p = dir_entry.path();
                    filesystem::path filename = p.filename();
                    auto name = filename.string();
                    if (name.ends_with(".texture.json")) {
                        load_texture_descriptor(p);
                    }
                }
            }
        }
    }

    void TextureManager::load_texture_descriptor(const filesystem::path& path) {
        if (filesystem::exists(path)) {
            string texture_descriptor_json = utils::read_file_to_string(path).value();
            json j = json::parse(texture_descriptor_json);

            TextureDescriptor texture_descriptor = j.template get<TextureDescriptor>();
            loaded_texture_descriptors[texture_descriptor.name] = texture_descriptor;
        }
    }

    optional<shared_ptr<Texture>> TextureManager::get_texture(WGPUDevice device, WGPUQueue queue, const string& id) {
        if (loaded_textures.find(id) != loaded_textures.end()) {
            return loaded_textures[id];
        }

        if (loaded_texture_descriptors.find(id) != loaded_texture_descriptors.end()) {
            const auto& desc = loaded_texture_descriptors[id];
            shared_ptr<Texture> texture = make_shared<Texture>(device, desc);

            // write texture data
            texture->write_texture(queue);

            loaded_textures[id] = texture;
            return texture;
        }

        return {};
    }

//    shared_ptr<Texture> TextureManager::create_render_texture(
//        WGPUDevice device,
//        const string& name,
//        int width,
//        int height,
//        int channel,
//        WGPUTextureFormat format
//    ) {
//        Texture texture{device, name, width, height, channel, format};
//        const string& id = texture.get_id();
//
//        shared_ptr<Texture> t = make_shared<Texture>(std::move(texture));
//        this->loaded_textures[id] = t;
//        return t;
//    }
}
