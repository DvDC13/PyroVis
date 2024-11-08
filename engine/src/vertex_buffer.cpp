#include "vertex_buffer.h"

namespace Pyro
{

    std::vector<VkVertexInputBindingDescription> Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }
    
    std::vector<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        return attributeDescriptions;
    }

    VertexBuffer::VertexBuffer(Device& device, const std::vector<Vertex>& vertices) : device_(device)
    {
        createVertexBuffer(vertices);
    }

    VertexBuffer::~VertexBuffer()
    {
        vkDestroyBuffer(device_.device(), vertexBuffer_, nullptr);
        vkFreeMemory(device_.device(), vertexBufferMemory_, nullptr);
    }

    void VertexBuffer::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = { vertexBuffer_ };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    void VertexBuffer::draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, vertexCount_, 1, 0, 0);
    }

    void VertexBuffer::createVertexBuffer(const std::vector<Vertex>& vertices)
    {
        vertexCount_ = static_cast<uint32_t>(vertices.size());
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        device_.createBuffer(bufferSize, 
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            vertexBuffer_,
            vertexBufferMemory_);

        void* data;
        vkMapMemory(device_.device(), vertexBufferMemory_, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t)bufferSize);
        vkUnmapMemory(device_.device(), vertexBufferMemory_);
    }
}