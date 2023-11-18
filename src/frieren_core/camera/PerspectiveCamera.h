#ifndef FRIEREN_CORE_PERSPECTIVE_CAMERA_H
#define FRIEREN_CORE_PERSPECTIVE_CAMERA_H

#include "Camera.h"
#include <common_include.h>

using namespace std;

namespace frieren_core {
    class PerspectiveCamera: public Camera {
    public:
        float fovy;
        float znear;
        float zfar;
        float aspect; // width / height
    public:
        PerspectiveCamera(float fovy, float aspect, float znear, float zfar);
        PerspectiveCamera() = default;

        glm::mat4x4 get_projection_matrix() const override;

        void set_perspective_parameters(float fovy, float aspect, float znear, float zfar);
    };

    class PerspectiveCameraController {
    public:
        shared_ptr<PerspectiveCamera> camera;

        float rotation_speed = 1;
        float translate_speed = 1;
    public:
        PerspectiveCameraController() = default;

        // void update_camera(PerspectiveCamera& camera) const;

        void move_camera_local(float dx, float dy);

        void move_around(float dx, float dy);
    };
}

#endif