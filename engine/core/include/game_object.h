#pragma once

#include "vertex_buffer.h"
#include "index_buffer.h"

#include <memory>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

namespace Pyro
{
    struct Transform
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};

        // translate * rotatex * rotatey * rotatez * scale
        // Y(1) X(2) Z(3) tait-bryan angles
        glm::mat4 mat4() const {
            auto transform = glm::translate(glm::mat4(1.0f), translation);
            transform = glm::rotate(transform, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            transform = glm::rotate(transform, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            transform = glm::rotate(transform, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::scale(transform, scale);
            return transform;
        }
    };

    struct Builder
    {
        std::shared_ptr<VertexBuffer> vertexBuffer_{};
        std::shared_ptr<IndexBuffer> indexBuffer_{};

        void loadModel(Device& device, const std::string& filepath);
    };

    class GameObject
    {
    public:
        static GameObject createObject();
        static GameObject createObjectfromFile(Device& device, const std::string& filepath);

        GameObject(const GameObject& other) = delete;
        GameObject& operator=(const GameObject& other) = delete;
        GameObject(GameObject&& other) = default;
        GameObject& operator=(GameObject&& other) = default;

        unsigned int id() const;

        void bind(VkCommandBuffer commandBuffer) const;

        glm::vec3 color_{};
        Transform transform_{};
        Builder builder_{};

    private:
        GameObject(unsigned int id);

        unsigned int id_ = 0;
    };
}