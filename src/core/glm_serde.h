#ifndef FRIEREN_CORE_GLM_SERDE_H
#define FRIEREN_CORE_GLM_SERDE_H

#include <common_include.h>
#include <common_include_glm.h>

namespace glm {
    void from_json(const json& j, glm::vec3& v);

    void from_json(const json& j, glm::vec2& v);

    void from_json(const json& j, glm:: vec4& v);

    void from_json(const json& j, glm::quat& quat);

    void from_json(const json& j, glm::mat4x4& mat);
}

#endif
