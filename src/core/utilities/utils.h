//
// Created by 58413 on 2023/10/17.
//

#ifndef FRIEREN_UTILS_H
#define FRIEREN_UTILS_H

#include <common_include.h>

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
                return name.substr(pos + 2);
            }
        }

        string get_json_object_any_key(const json& j);
    }
}

#endif //FRIEREN_UTILS_H
