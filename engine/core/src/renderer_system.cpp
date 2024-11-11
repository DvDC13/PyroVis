#include "renderer_system.h"

namespace Pyro
{
    RendererSystem::RendererSystem(Device& device, VkRenderPass renderPass) : device_(device)
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    RendererSystem::~RendererSystem()
    {
        vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
    }

    void RendererSystem::renderGameObjects(VkCommandBuffer commandBuffer, const std::vector<GameObject>& gameObjects) {

        pipeline_->bind(commandBuffer);

        for (auto& object : gameObjects) { 
            
            PushConstants push{};
            push.color = object.color_;
            push.transform = object.transform_.mat4();

            vkCmdPushConstants(commandBuffer,
                            pipelineLayout_,
                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                            0,
                            sizeof(PushConstants),
                            &push
                            );
            
            object.vertexBuffer_->bind(commandBuffer);
            object.vertexBuffer_->draw(commandBuffer);
        }
    }

    void RendererSystem::createPipelineLayout() {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.size = sizeof(PushConstants);
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

        if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void RendererSystem::createPipeline(VkRenderPass renderPass) {
        
        PipelineConfigInfo pipelineConfigInfo{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfigInfo);

        pipelineConfigInfo.renderPass = renderPass;
        pipelineConfigInfo.pipelineLayout = pipelineLayout_;

        pipeline_ = std::make_unique<Pipeline>(device_, "../shaders/triangle.vert.spv", "../shaders/triangle.frag.spv", pipelineConfigInfo);

    }
}