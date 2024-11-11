#pragma once

#include "device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cstring>

namespace Pyro
{

    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
    };

    class VertexBuffer
    {
    public:
        VertexBuffer(Device& device, const std::vector<Vertex>& vertices);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = default;
        VertexBuffer& operator=(VertexBuffer&&) = default;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        Device& device_;
        VkBuffer vertexBuffer_;
        VkDeviceMemory vertexBufferMemory_;
        uint32_t vertexCount_;

        void createVertexBuffer(const std::vector<Vertex>& vertices);

    };
}