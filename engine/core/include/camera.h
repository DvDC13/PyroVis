#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Pyro
{
    class Camera
    {
    public:
        Camera() = default;
        ~Camera() = default;

        void setOrthographicProjection(float left, float right, float top, float bottom, float near, float far);
        void setPerspectiveProjection(float fov, float aspect, float near, float far);

        void lookAtDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
        void lookAtTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.0f, -1.0f, 0.0f));
        void lookAtInYXZ(glm::vec3 position, glm::vec3 rotation); // Tait-Bryan angles

        const glm::mat4& getProjectionMatrix() const { return projection_matrix_; }
        const glm::mat4& getViewMatrix() const { return view_matrix_; }

    private:
        glm::mat4 projection_matrix_ = glm::mat4(1.0f);
        glm::mat4 view_matrix_ = glm::mat4(1.0f);
    };
}