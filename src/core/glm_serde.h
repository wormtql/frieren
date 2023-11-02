#ifndef FRIEREN_CORE_GLM_SERDE_H
#define FRIEREN_CORE_GLM_SERDE_H

#include <common_include.h>
#include <common_include_glm.h>

namespace glm {
    void from_json(const json& j, glm::vec3& v);

    void from_json(const json& j, glm::quat& quat);
}

#endif
