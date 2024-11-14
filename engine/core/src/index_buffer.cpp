#include "index_buffer.h"

namespace Pyro
{
    IndexBuffer::IndexBuffer(Device& device, const VertexBuffer& vertexBuffer, const std::vector<uint32_t>& indices)
        : device_(device), vertexBuffer_(vertexBuffer)
    {
        createIndexBuffer(indices);
    }

    IndexBuffer::~IndexBuffer()
    {
        vkDestroyBuffer(device_.device(), indexBuffer_, nullptr);
        vkFreeMemory(device_.device(), indexBufferMemory_, nullptr);
    }

    void IndexBuffer::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { vertexBuffer_.getVertexBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
    }

    void IndexBuffer::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDrawIndexed(commandBuffer, indexCount_, 1, 0, 0, 0);
    }

    void IndexBuffer::createIndexBuffer(const std::vector<uint32_t>& indices)
    {
        indexCount_ = static_cast<uint32_t>(indices.size());
        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount_;

        device_.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            indexBuffer_,
            indexBufferMemory_);

        void* data;
        vkMapMemory(device_.device(), indexBufferMemory_, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(device_.device(), indexBufferMemory_);
    }
}