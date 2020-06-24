#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
namespace Cecilion {
    /**
     * Untested camera class
     */
    class Camera {
    public:
        void set_position(const glm::vec3& position) {m_position = position;}
        void set_rotation(float x, float y, float z, float w) {
            this->m_rotation.x = x;
            this->m_rotation.y = y;
            this->m_rotation.z = z;
            this->m_rotation.w = w;
        }
        const glm::vec3& get_position() {return this->m_position;}
        const glm::quat get_rotation() {return this->m_rotation;}
        const glm::mat4& get_projection_matrix() const {return this->m_projection_matrix;}
        const glm::mat4& get_view_matrix() const {return this->m_view_matrix;}
        const glm::mat4& get_view_projection_matrix() const {return this->m_view_projection_matrix;}
        virtual void on_update() = 0;
        virtual void look_at(glm::vec3& position) = 0;
    protected:
        glm::mat4 m_projection_matrix;
        glm::mat4 m_view_matrix;
        glm::mat4 m_view_projection_matrix;
        glm::vec3 m_position;
        glm::vec3 m_viewpoint;
        glm::vec3 m_up_vector;
        glm::quat m_rotation;
    };

    class Perspective_camera : public Camera{
    public:
        Perspective_camera(float znear, float zfar, float fov, float aspect_ratio);
        void on_update() override;

        void look_at(glm::vec3 &position) override;

    private:
    };

    class Orthographic_camera : public Camera{
    public:
        Orthographic_camera(float left, float right, float top, float bottom);
        void on_update() override;

        void look_at(glm::vec3 &position) override;

    private:

        float rotation = 0.0f;
    };
}