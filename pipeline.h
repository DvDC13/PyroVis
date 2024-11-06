#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>

#include "device.h"

namespace Pyro
{
    struct PipelineConfigInfo
    {
        VkViewport viewport;
        VkRect2D scissor;
        
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;
        VkRenderPass renderPass = VK_NULL_HANDLE;
        uint32_t subpass = 0;
    };

    class Pipeline
    {
    public:
        Pipeline(Device& device, const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const PipelineConfigInfo& configInfo);
        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&&) = default;
        Pipeline& operator=(Pipeline&&) = default;

        void bind(VkCommandBuffer commandBuffer);

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        Device& device_;
        VkPipeline graphicsPipeline_;
        VkShaderModule vertShaderModule_;
        VkShaderModule fragShaderModule_;

        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
    };
}