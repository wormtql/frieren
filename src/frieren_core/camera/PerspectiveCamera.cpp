#include "PerspectiveCamera.h"
#include <common_include_glm.h>

namespace frieren_core {
    PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float znear, float zfar): Camera{} {
        this->position = { 0, 0, 1 };
        this->rotation = { 0, 0, 0 };
        this->set_perspective_parameters(fovy, aspect, znear, zfar);
    }

    void PerspectiveCamera::set_perspective_parameters(float fovy, float aspect, float znear, float zfar) {
        this->fovy = fovy;
        this->aspect = aspect;
        this->znear = znear;
        this->zfar = zfar;
    }

    glm::mat4x4 PerspectiveCamera::get_projection_matrix() const {
        auto opengl_projection_mat = glm::perspective(this->fovy, this->aspect, this->znear, this->zfar);

        static glm::mat4x4 opengl_to_wgpu{
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.5,
            0.0, 0.0, 0.0, 1.0,
        };

        return opengl_to_wgpu * opengl_projection_mat;
    }

    void PerspectiveCameraController::move_camera_local(float dx, float dy) {
        auto rotation_matrix = camera->get_rotation_matrix();

        glm::vec3 camera_right = rotation_matrix * glm::vec4{ 1, 0, 0, 1 };
        glm::vec3 camera_top = rotation_matrix * glm::vec4{ 0, 1, 0, 1 };
        glm::vec3 camera_front = rotation_matrix * -glm::vec4{ 0, 0, 1, 1 };

        glm::vec3 position = camera->position;
        position += dx * camera_right * translate_speed;
        position += dy * camera_front * translate_speed;

        camera->position = position;

        auto mat = camera->get_view_matrix();
        // cout << camera->get_view_matrix() << endl;
    }

    // void PerspectiveCameraController::update_camera(PerspectiveCamera& camera) const {
    //     // this is the matrix convert from world axis to camera axis
    //     auto rotation_matrix = camera.get_rotation_matrix();

    //     glm::vec3 camera_right = rotation_matrix * glm::vec4{ 1, 0, 0, 1 };
    //     glm::vec3 camera_top = rotation_matrix * glm::vec4{ 0, 1, 0, 1 };
    //     glm::vec3 camera_front = rotation_matrix * glm::vec4{ 0, 0, 1, 1 };

    //     glm::vec3 position = camera.position;
    //     position += this->left * (-camera_right) * this->translate_speed;
    //     position += this->forward * camera_front * this->translate_speed;

    //     camera.position = position;

    //     camera.rotation.x += this->dy * this->rotation_speed;
    //     camera.rotation.y += this->dx * this->rotation_speed;
    // }
}