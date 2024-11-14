#pragma once

#include "device.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cstring>

#include "vertex_buffer.h"

namespace Pyro
{
    class IndexBuffer
    {
    public:
        IndexBuffer(Device& device, const VertexBuffer& vertexBuffer, const std::vector<uint32_t>& indices);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = default;
        IndexBuffer& operator=(IndexBuffer&&) = default;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        Device& device_;
        VkBuffer indexBuffer_;
        VkDeviceMemory indexBufferMemory_;
        uint32_t indexCount_;

        const VertexBuffer& vertexBuffer_;

        void createIndexBuffer(const std::vector<uint32_t>& indices);

    };
}