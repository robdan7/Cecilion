#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
namespace Cecilion {
    /**
     * Untested camera class
     */
    class Camera {
    public:
        virtual void set_position(const glm::vec3& position) { m_position = position;}


         /**
          *
          * @param axis - A normalized axis
          * @param angle - The angle givenin radians.
          */
        virtual void set_rotation(glm::vec3 axis, float angle) {
            this->m_rotation.x = axis.x * sin(angle / 2);
            this->m_rotation.y = axis.y * sin(angle / 2);
            this->m_rotation.z = axis.z * sin(angle / 2);
            this->m_rotation.w = cos(angle / 2);

//            w = cos(RotationAngle / 2)
//            this->m_rotation = rotation;
        }

        virtual void set_rotation(float pitch, float yaw, float roll) {
            double cy = cos(roll * 0.5);
            double sy = sin(roll * 0.5);
            double cp = cos(pitch * 0.5);
            double sp = sin(pitch * 0.5);
            double cr = cos(yaw * 0.5);
            double sr = sin(yaw * 0.5);

            this->m_rotation.w = cr * cp * cy + sr * sp * sy;
            this->m_rotation.x = sr * cp * cy - cr * sp * sy;
            this->m_rotation.y = cr * sp * cy + sr * cp * sy;
            this->m_rotation.z = cr * cp * sy - sr * sp * cy;
        }

        const glm::vec3& get_position() {return this->m_position;}
        const glm::quat& get_rotation() {return this->m_rotation;}
        const glm::mat4& get_projection_matrix() const {return this->m_projection_matrix;}
        const glm::mat4& get_view_matrix() const {return this->m_view_matrix;}
        const glm::mat4& get_view_projection_matrix() const {return this->m_view_projection_matrix;}
        virtual void on_update() = 0;
        virtual void look_at(const glm::vec3& position) = 0;
    protected:
        glm::mat4 m_projection_matrix;
        glm::mat4 m_view_matrix;
        glm::mat4 m_view_projection_matrix;
        glm::vec3 m_focus;
        glm::vec3 m_position;
        glm::vec3 m_up_vector;
        glm::quat m_rotation;
    };

    class Perspective_camera : public Camera{
    public:
        Perspective_camera(float znear, float zfar, float fov, float aspect_ratio);
        void on_update() override;

        /**
         * Set the rotation of this camera. This function will effectively override any previous call to look_at().
         * @param rotation
         */
        void set_rotation(glm::vec3 axis, float angle) override;

        void set_rotation(float pitch, float yaw, float roll) override;

        /**
         * Look at a fixed point in euclidean space. This function will effectively override any previous call to
         * set_rotation().
         * @param position
         */
        void look_at(const glm::vec3 &position) override;

    private:
    };

    class Orthographic_camera : public Camera{
    public:
        Orthographic_camera(float left, float right, float top, float bottom, float znear, float zfar);
        void on_update() override;

        void look_at(const glm::vec3 &position) override;

    private:

        float rotation = 0.0f;
    };
}