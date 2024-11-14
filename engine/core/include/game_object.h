#pragma once

#include "vertex_buffer.h"
#include "index_buffer.h"
#include <memory>

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

        void bind(VkCommandBuffer commandBuffer) const
        {
            VkBuffer buffers[] = { vertexBuffer_->getVertexBuffer() };
            VkDeviceSize offsets[] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
            
            if (indexBuffer_ != nullptr)
                vkCmdBindIndexBuffer(commandBuffer, indexBuffer_->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
        }

        std::shared_ptr<VertexBuffer> vertexBuffer_{};
        std::shared_ptr<IndexBuffer> indexBuffer_{};
        glm::vec3 color_{};
        Transform transform_{};

    private:
        GameObject(unsigned int id) : id_(id) {}

        unsigned int id_ = 0;
    };
}