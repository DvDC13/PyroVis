#include "application.h"

namespace Pyro
{

    Application::Application()
    {
        loadVertexBuffer();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    Application::~Application()
    {
        vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
    }

    void Application::run()
    {
        while (!window_.isClosed())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(device_.device());
    }

    void Application::loadVertexBuffer()
    {
        std::vector<Vertex> vertices = {
            {{-0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        vertexBuffer_ = std::make_unique<VertexBuffer>(device_, vertices);
    }

    void Application::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void Application::createPipeline()
    {
        auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain_.width(), swapChain_.height());
        pipelineConfig.renderPass = swapChain_.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout_;

        pipeline_ = std::make_unique<Pipeline>(device_, "../shaders/triangle.vert.spv", "../shaders/triangle.frag.spv", pipelineConfig);
    }

    void Application::recordCommandBuffers() {

        for (size_t i = 0; i < commandBuffers_.size(); i++) {

            vkResetCommandBuffer(commandBuffers_[i], 0);

            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(commandBuffers_[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain_.getRenderPass();
            renderPassInfo.framebuffer = swapChain_.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain_.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers_[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline_->bind(commandBuffers_[i]);

            vertexBuffer_->bind(commandBuffers_[i]);
            vertexBuffer_->draw(commandBuffers_[i]);

            vkCmdEndRenderPass(commandBuffers_[i]);

            if (vkEndCommandBuffer(commandBuffers_[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }

    void Application::createCommandBuffers()
    {
        commandBuffers_.resize(swapChain_.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = device_.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

        if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        recordCommandBuffers();
    }

    void Application::drawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain_.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        result = swapChain_.submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }
}