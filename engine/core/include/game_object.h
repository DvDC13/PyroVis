#pragma once

#include "vertex_buffer.h"
#include <memory>

namespace Pyro
{
    struct Transform2D
    {
        glm::vec2 translation{};
        glm::vec2 scale{1.0f};
        float rotation{0.0f};
        glm::mat2 mat2() const {
            glm::mat2 m_rot = glm::mat2(cos(rotation), sin(rotation), -sin(rotation), cos(rotation)); 
            glm::mat2 m_scale = glm::mat2(scale.x, 0.0f, 0.0f, scale.y);
            return m_rot * m_scale;
        }
    };

    class GameObject
    {
    public:
        static GameObject create() { 
            static unsigned int curr = 0;
            return GameObject(curr++);
        }

        GameObject(const GameObject& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject(GameObject&& other) = default;
        GameObject& operator=(GameObject&& other) = default;

        inline unsigned int id() const { return id_; }

        std::shared_ptr<VertexBuffer> vertexBuffer_{};
        glm::vec3 color_{};
        Transform2D transform2D_{};

    private:
        GameObject(unsigned int id) : id_(id) {}

        unsigned int id_ = 0;
    };
}