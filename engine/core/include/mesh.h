#pragma once

#include "buffer.h"
#include "hash.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

namespace Pyro
{
    struct Vertex {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec2 texCoord_uv{};
        glm::vec3 normal{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color && texCoord_uv == other.texCoord_uv && normal == other.normal;
        }
    };

    struct Builder
    {
        std::vector<Vertex> vertices{};
        std::vector<uint32_t> indices{};

        void loadModel(const std::string& filepath);
    };

    class Mesh
    {
    public:
        Mesh(Device& device, const Builder& builder);
        ~Mesh();

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh(Mesh&&) = default;
        Mesh& operator=(Mesh&&) = default;

        static std::shared_ptr<Mesh> createMeshFromFile(Device& device, const std::string& filepath);

        void bind(VkCommandBuffer commandBuffer) const;
        void draw(VkCommandBuffer commandBuffer) const;

        void createVertexBuffer(const std::vector<Vertex>& vertices);
        void createIndexBuffer(const std::vector<uint32_t>& indices);

    private:
        Device& device_;

        std::unique_ptr<Buffer> vertexBuffer_;
        uint32_t vertexCount_ = 0;

        std::unique_ptr<Buffer> indexBuffer_;
        uint32_t indexCount_ = 0;
        bool hasIndexBuffer_ = false;
    };
}