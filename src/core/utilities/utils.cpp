#include "utils.h"

namespace fieren_core::utils {
    optional<string> read_file_to_string(const std::filesystem::path& path) {
        std::ifstream t(path);

        if (!t.is_open()) {
            return {};
        }

        std::stringstream buffer;
        buffer << t.rdbuf();

        return std::move(buffer.str());
    }

    string get_json_object_any_key(const json& j) {
        assert(j.is_object());
        for (const auto& p: j.items()) {
            return p.key();
        }
        assert(false);
        return "";
    }
}
