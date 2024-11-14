#pragma once

#include "game_object.h"
#include "window.h"

namespace Pyro
{
    class Keyboard
    {
    public:
        Keyboard() = default;
        ~Keyboard() = default;
        Keyboard(const Keyboard&) = delete;
        Keyboard& operator=(const Keyboard&) = delete;
        Keyboard(Keyboard&&) = delete;
        Keyboard& operator=(Keyboard&&) = delete;

        struct KeyMap
        {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBackward = GLFW_KEY_S;
            int moveUp = GLFW_KEY_E;
            int moveDown = GLFW_KEY_Q;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        void moveInPlaneXZ(GLFWwindow* window, float dt, GameObject& gameObject) {
            glm::vec3 rotate{0.0f};

            if (glfwGetKey(window, keyMap_.lookLeft) == GLFW_PRESS)
                rotate.y -= 1.0f;
            if (glfwGetKey(window, keyMap_.lookRight) == GLFW_PRESS)
                rotate.y += 1.0f;
            if (glfwGetKey(window, keyMap_.lookUp) == GLFW_PRESS)
                rotate.x += 1.0f;
            if (glfwGetKey(window, keyMap_.lookDown) == GLFW_PRESS)
                rotate.x -= 1.0f;

            if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
                gameObject.transform_.rotation += glm::normalize(rotate) * lookSpeed_ * dt;

            // limit pitch
            gameObject.transform_.rotation.x = glm::clamp(gameObject.transform_.rotation.x, -1.5f, 1.5f);
            gameObject.transform_.rotation.y = glm::mod(gameObject.transform_.rotation.y, glm::two_pi<float>());

            float yaw = gameObject.transform_.rotation.y;
            float directionX = glm::sin(yaw);
            float directionZ = glm::cos(yaw);
            const glm::vec3 forwardDir(directionX, 0.0f, directionZ);
            const glm::vec3 rightDir = glm::vec3(forwardDir.z, 0.0f, -forwardDir.x);
            const glm::vec3 upDir = glm::vec3(0.0f, -1.0f, 0.0f);

            glm::vec3 move{0.0f};

            if (glfwGetKey(window, keyMap_.moveLeft) == GLFW_PRESS)
                move -= rightDir;
            if (glfwGetKey(window, keyMap_.moveRight) == GLFW_PRESS)
                move += rightDir;
            if (glfwGetKey(window, keyMap_.moveForward) == GLFW_PRESS)
                move += forwardDir;
            if (glfwGetKey(window, keyMap_.moveBackward) == GLFW_PRESS)
                move -= forwardDir;
            if (glfwGetKey(window, keyMap_.moveUp) == GLFW_PRESS)
                move += upDir;
            if (glfwGetKey(window, keyMap_.moveDown) == GLFW_PRESS)
                move -= upDir;

            if (glm::dot(move, move) > std::numeric_limits<float>::epsilon())
                gameObject.transform_.translation += glm::normalize(move) * moveSpeed_ * dt;
        }

        KeyMap keyMap_{};
        float moveSpeed_ = 3.0f;
        float lookSpeed_ = 1.5f;
    };
}