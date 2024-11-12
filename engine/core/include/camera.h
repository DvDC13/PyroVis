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

        const glm::mat4& getProjectionMatrix() const { return projection_matrix_; }

    private:
        glm::mat4 projection_matrix_ = glm::mat4(1.0f);
    };
}