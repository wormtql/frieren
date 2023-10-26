#include "utils.h"

namespace fieren_core {
    namespace utils {
        optional<string> read_file_to_string(const std::filesystem::path& path) {
            std::ifstream t(path);

            if (!t.is_open()) {
                return {};
            }

            std::stringstream buffer;
            buffer << t.rdbuf();

            return std::move(buffer.str());
        }
    }
}