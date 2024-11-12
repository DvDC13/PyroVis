#include "camera.h"

namespace Pyro
{
    void Camera::setOrthographicProjection(float left, float right, float top, float bottom, float near, float far)
    {
        projection_matrix_ = glm::mat4(1.0f);
        projection_matrix_[0][0] = 2.0f / (right - left);
        projection_matrix_[1][1] = 2.0f / (bottom - top);
        projection_matrix_[2][2] = 1.0f / (far - near);
        projection_matrix_[3][0] = -(right + left) / (right - left);
        projection_matrix_[3][1] = -(bottom + top) / (bottom - top);
        projection_matrix_[3][2] = -near / (far - near);

    }
    void Camera::setPerspectiveProjection(float fov, float aspect, float near, float far)
    {
        projection_matrix_ = glm::mat4(1.0f);
        projection_matrix_[0][0] = 1.0f / (aspect * tan(glm::radians(fov) / 2.0f));
        projection_matrix_[1][1] = 1.0f / (tan(glm::radians(fov) / 2.0f));
        projection_matrix_[2][2] = far / (far - near);
        projection_matrix_[2][3] = 1.0f;
        projection_matrix_[3][2] = - (far * near) / (far - near);
    }
}