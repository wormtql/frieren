#include "Camera.h"

namespace frieren_core {
    glm::mat4x4 Camera::get_view_matrix() const {
        glm::mat4x4 m{1.0};
        m = glm::translate(m, -this->position);
        
        glm::mat4x4 rot = glm::eulerAngleXYZ(this->rotation.x, this->rotation.y, this->rotation.z);
        return glm::transpose(rot) * m;
    }

    glm::mat4x4 Camera::get_rotation_matrix() const {
        return glm::eulerAngleXYZ(this->rotation.x, rotation.y, rotation.z);
    }
}
