#include "index_buffer.h"

namespace Pyro
{
    IndexBuffer::IndexBuffer(Device& device, const std::vector<uint32_t>& indices)
        : device_(device)
    {
        createIndexBuffer(indices);
    }

    IndexBuffer::~IndexBuffer()
    {
        vkDestroyBuffer(device_.device(), indexBuffer_, nullptr);
        vkFreeMemory(device_.device(), indexBufferMemory_, nullptr);
    }

    void IndexBuffer::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDrawIndexed(commandBuffer, indexCount_, 1, 0, 0, 0);
    }

    void IndexBuffer::createIndexBuffer(const std::vector<uint32_t>& indices)
    {
        indexCount_ = static_cast<uint32_t>(indices.size());
        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount_;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        device_.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory);

        void* data;
        vkMapMemory(device_.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(device_.device(), stagingBufferMemory);

        device_.createBuffer(bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer_,
            indexBufferMemory_);

        device_.copyBuffer(stagingBuffer, indexBuffer_, bufferSize);

        vkDestroyBuffer(device_.device(), stagingBuffer, nullptr);
        vkFreeMemory(device_.device(), stagingBufferMemory, nullptr);
    }
}