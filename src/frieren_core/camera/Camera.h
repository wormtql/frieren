#ifndef FRIEREN_CORE_CAMERA_H
#define FRIEREN_CORE_CAMERA_H

#include <common_include_glm.h>

namespace frieren_core {
    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 rotation;
        
    
    public:
        glm::mat4x4 get_view_matrix() const;
    };
}

#endif
