#ifndef FRIEREN_CORE_UTILS_H
#define FRIEREN_CORE_UTILS_H

#include <common_include.h>
#include <webgpu/webgpu.hpp>

using namespace std;

namespace frieren_core {
    template<typename T>
    size_t hash(const T& value) {
        auto size = sizeof(T);
        string s{(char*)&value, size};
        return std::hash<string>{}(s);
    }

    namespace utils {
        optional<string> read_file_to_string(const std::filesystem::path& path);

        template<typename T>
        string get_type_name() {
            string name = typeid(T).name();
            auto pos = name.find_last_of("::");
            if (pos == string::npos) {
                return name;
            } else {
                return name.substr(pos + 1);
            }
        }

        string get_json_object_any_key(const json& j);

        int get_wgpu_texture_format_pixel_size(WGPUTextureFormat format);
    }
}

#endif //FRIEREN_UTILS_H
