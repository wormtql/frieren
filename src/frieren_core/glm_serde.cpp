#include "glm_serde.h"

namespace glm {
    void from_json(const json& j, glm::vec3& v) {
        assert(j.is_array());
        v.x = j[0];
        v.y = j[1];
        v.z = j[2];
    }

    void from_json(const json& j, glm::vec2& v) {
        assert(j.is_array());
        v.x = j[0];
        v.y = v[1];
    }

    void from_json(const json& j, glm::vec4& v) {
        assert(j.is_array());
        v.x = j[0];
        v.y = j[1];
        v.z = j[2];
        v.w = j[3];
    }

    void from_json(const json& j, glm::mat4x4& mat) {
        assert(j.is_array());
        for (int i = 0; i < 16; i++) {
            mat[i] = j[i];
        }
    }
}
