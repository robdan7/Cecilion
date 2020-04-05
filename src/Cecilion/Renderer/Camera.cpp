#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace Cecilion {

    /// ---------- Perspective camera ----------

    Perspective_camera::Perspective_camera(float znear, float zfar, float fov, float aspect_ratio) {
        this->m_up_vector = {0,1,0};
        this->m_position = {0,0,0};
        this->m_rotation = {this->m_up_vector.x,this->m_up_vector.y,this->m_up_vector.z,0};
        this->m_viewpoint = {0,0,1};
        this->m_projection_matrix = glm::perspective(fov,aspect_ratio,znear,zfar);
        this->m_view_projection_matrix = this->m_projection_matrix * this->m_view_matrix;
    }

    void Perspective_camera::on_update() {
        this->m_view_matrix = glm::lookAt(this->m_position, this->m_viewpoint, this->m_up_vector);
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(this->m_rotation);
        m_view_matrix = glm::inverse(transform);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
        // TODO maaaaybe this should be in a generic method instead.
    }

    void Perspective_camera::look_at(glm::vec3 &position) {
        this->m_viewpoint = position;

    }

    /// ---------- Orthographic camera ----------

    Orthographic_camera::Orthographic_camera(float left, float right, float top, float bottom) {
        this->m_projection_matrix = glm::ortho(left, right, bottom, top);
        this->m_view_projection_matrix = this->m_projection_matrix * this->m_view_matrix;
    }

    void Orthographic_camera::on_update() {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(this->m_rotation);
        m_view_matrix = glm::inverse(transform);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

    void Orthographic_camera::look_at(glm::vec3 &position) {
        this->m_position = position;
    }
}