#pragma once

#include "pipeline.h"
#include "device.h"
#include "logger.h"
#include "game_object.h"
#include "camera.h"

#include <memory>
#include <array>

namespace Pyro
{
    struct PushConstants
    {
        glm::mat4 transform{1.0f};
        glm::mat4 modelMatrix{1.0f};
    };

    class RendererSystem
    {
    public:
        RendererSystem(Device& device, VkRenderPass renderPass);
        ~RendererSystem();

        RendererSystem(const RendererSystem&) = delete;
        RendererSystem& operator=(const RendererSystem&) = delete;
        RendererSystem(RendererSystem&&) = delete;
        RendererSystem& operator=(RendererSystem&&) = delete;
        
        void renderGameObjects(VkCommandBuffer commandBuffer, const std::vector<GameObject>& gameObjects, const Camera& camera);

    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device& device_;
        std::unique_ptr<Pipeline> pipeline_;
        VkPipelineLayout pipelineLayout_;
    };
}