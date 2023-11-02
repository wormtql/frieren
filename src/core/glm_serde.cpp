#include "glm_serde.h"

namespace glm {
    void from_json(const json& j, glm::vec3& v) {
        assert(j.is_array());
        v.x = j[0];
        v.y = j[1];
        v.z = j[2];
    }
}
