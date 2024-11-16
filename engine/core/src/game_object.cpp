#include "game_object.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Pyro
{

    void Builder::loadModel(Device& device, const std::string& filepath) {

        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn;
        std::string err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
            throw std::runtime_error(warn + err);
        }

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex = {};
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                };

                vertex.texCoord_uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                };

                vertex.color = {1.0f, 1.0f, 1.0f};
                vertices.push_back(vertex);
                indices.push_back(index.vertex_index);
            }
        }

        vertexBuffer_ = std::make_shared<VertexBuffer>(device, vertices);
        indexBuffer_ = std::make_shared<IndexBuffer>(device, indices);
    }

    GameObject GameObject::createObject() { 
        static unsigned int curr = 0;
        return GameObject(curr++);
    }

    GameObject GameObject::createObjectfromFile(Device& device, const std::string& filepath) {
        Builder builder_{};
        builder_.loadModel(device, filepath);
        std::cout << "loaded model: " << filepath << std::endl;
        std::cout << "Vertices: " << builder_.vertexBuffer_->getVertexCount() << std::endl;
        std::cout << "Indices: " << builder_.indexBuffer_->getIndexCount() << std::endl;
        auto gameObject = GameObject::createObject();
        gameObject.builder_ = builder_;
        return gameObject;
    }

    unsigned int GameObject::id() const { return id_; }

    void GameObject::bind(VkCommandBuffer commandBuffer) const
    {
        VkBuffer buffers[] = { builder_.vertexBuffer_->getVertexBuffer() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        
        if (builder_.indexBuffer_ != nullptr)
            vkCmdBindIndexBuffer(commandBuffer, builder_.indexBuffer_->getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
    }

    GameObject::GameObject(unsigned int id) : id_(id) {}
};
