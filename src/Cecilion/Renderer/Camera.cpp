#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Core/Log.h>
namespace Cecilion {

    /// ---------- Perspective camera ----------

    Perspective_camera::Perspective_camera(float znear, float zfar, float fov, float aspect_ratio) {
        this->m_up_vector = {0,1,0};
        this->m_focus = {0, 0, 0};
        this->m_rotation = {0,this->m_up_vector.x,this->m_up_vector.y,this->m_up_vector.z};
        this->m_position = {0, 0, -1};
        this->m_projection_matrix = glm::perspective(fov,aspect_ratio,znear,zfar);
        this->m_view_projection_matrix = this->m_projection_matrix * this->m_view_matrix;
    }

    void Perspective_camera::on_update() {
        this->m_view_matrix = glm::lookAt(this->m_position,this->m_focus, this->m_up_vector);
//        this->m_transformation_matrix = glm::inverse(glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(this->m_rotation));
//        auto m_transformation_matrix =  glm::translate(m_view_matrix, -m_position);
//        m_view_matrix = glm::inverse(transform);

        m_view_projection_matrix = m_projection_matrix * (m_view_matrix);
        // TODO maaaaybe this should be in a generic method instead.
    }

    void Perspective_camera::look_at(const glm::vec3 &position) {
        this->m_focus = position;

    }

    void Perspective_camera::set_rotation(glm::vec3 axis, float angle) {
        Camera::set_rotation(axis, angle);
//        this->m_focus = glm::rotate(this->m_rotation,glm::vec4(0,1,0,0));
        this->m_focus = this->m_position + (glm::vec3)(glm::toMat4(this->m_rotation)* glm::vec4 (0,0,-1,0));
//        CORE_LOG_INFO("m_focus: {0}, {0}, {0}", this->m_focus.x, this->m_focus.y, this->m_focus.z);
//        this->look_at((glm::vec3)(glm::toMat4(this->m_rotation)*glm::vec4(0,0,-1,0)) + this->m_position);
    }

    void Perspective_camera::set_rotation(float pitch, float yaw, float roll) {
        Camera::set_rotation(pitch, yaw, roll);
        this->m_focus = this->m_position + (glm::vec3)(glm::toMat4(this->m_rotation)* glm::vec4 (0,0,-1,0));
    }

    /// ---------- Orthographic camera ----------

    Orthographic_camera::Orthographic_camera(float left, float right, float top, float bottom, float znear, float zfar) {
        this->m_projection_matrix = glm::ortho(left, right, bottom, top, znear, zfar);
        this->m_view_projection_matrix = this->m_projection_matrix * this->m_view_matrix;
    }

    void Orthographic_camera::on_update() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_focus) * glm::toMat4(this->m_rotation);
        m_view_matrix = glm::inverse(transform);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

    void Orthographic_camera::look_at(const glm::vec3 &position) {
        this->m_focus = position;
    }
}