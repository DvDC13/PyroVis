#pragma once

#include "pipeline.h"
#include "device.h"
#include "logger.h"
#include "game_object.h"
#include "camera.h"

#include <memory>
#include <array>

#include "imconfig.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace Pyro
{
    struct FrameInfo {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        Camera& camera;
        VkDescriptorSet descriptorSet;
    };

    struct PushConstants
    {
        glm::mat4 modelMatrix{1.0f};
    };

    class RendererSystem
    {
    public:
        RendererSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout);
        ~RendererSystem();

        RendererSystem(const RendererSystem&) = delete;
        RendererSystem& operator=(const RendererSystem&) = delete;
        RendererSystem(RendererSystem&&) = delete;
        RendererSystem& operator=(RendererSystem&&) = delete;
        
        void renderGameObjects(FrameInfo& frameInfo, const std::vector<GameObject>& gameObjects);

    private:
        void createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout);
        void createPipeline(VkRenderPass renderPass);

        Device& device_;
        std::unique_ptr<Pipeline> pipeline_;
        VkPipelineLayout pipelineLayout_;
    };
}