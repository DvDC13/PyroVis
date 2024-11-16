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
        IndexBuffer(Device& device, const std::vector<uint32_t>& indices);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = default;
        IndexBuffer& operator=(IndexBuffer&&) = default;

        void draw(VkCommandBuffer commandBuffer);

        inline const VkBuffer& getIndexBuffer() const { return indexBuffer_; }
        inline uint32_t getIndexCount() const { return indexCount_; }

    private:
        Device& device_;
        VkBuffer indexBuffer_;
        VkDeviceMemory indexBufferMemory_;
        uint32_t indexCount_;

        void createIndexBuffer(const std::vector<uint32_t>& indices);

    };
}