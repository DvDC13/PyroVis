#include "renderer_system.h"

namespace Pyro
{
    RendererSystem::RendererSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout) : device_(device)
    {
        createPipelineLayout(descriptorSetLayout);
        createPipeline(renderPass);
    }

    RendererSystem::~RendererSystem()
    {
        vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
    }

    void RendererSystem::renderGameObjects(FrameInfo& frameInfo, const std::vector<GameObject>& gameObjects) {

        pipeline_->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(frameInfo.commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipelineLayout_,
                                0,
                                1,
                                &frameInfo.descriptorSet,
                                0,
                                nullptr);

        for (auto& object : gameObjects) { 
            
            PushConstants push{};
            auto modelMatrix = object.transform_.mat4();
            push.modelMatrix = modelMatrix;

            vkCmdPushConstants(frameInfo.commandBuffer,
                            pipelineLayout_,
                            VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                            0,
                            sizeof(PushConstants),
                            &push
                            );
            
            object.mesh_->bind(frameInfo.commandBuffer);
            object.mesh_->draw(frameInfo.commandBuffer);
        }
    }

    void RendererSystem::createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout) {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.size = sizeof(PushConstants);
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ descriptorSetLayout };

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
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