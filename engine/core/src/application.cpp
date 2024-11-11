#include "application.h"

namespace Pyro
{

    Application::Application()
    {
        Logger logger;
        logger.log(Logger::LogLevel::INFO, "Application started");

        loadGameObjects();
        createPipelineLayout();
        recreateSwapChain();
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

    void Application::loadGameObjects()
    {
        std::vector<Vertex> vertices = {
            {{-0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        };

        auto vertexBuffer_ = std::make_shared<VertexBuffer>(device_, vertices);

        auto triangle = GameObject::create();
        triangle.vertexBuffer_ = vertexBuffer_;
        triangle.color_ = {1.0f, 0.0f, 0.0f};
        triangle.transform2D_.translation.x = 0.2f;
        triangle.transform2D_.scale = {0.2f, 0.5f};
        triangle.transform2D_.rotation = 0.25f * glm::two_pi<float>();

        gameObjects_.push_back(std::move(triangle));
    }

    void Application::createPipelineLayout()
    {
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

    void Application::createPipeline()
    {
        PipelineConfigInfo pipelineConfigInfo{};
        Pipeline::defaultPipelineConfigInfo(pipelineConfigInfo);

        pipelineConfigInfo.renderPass = swapChain_->getRenderPass();
        pipelineConfigInfo.pipelineLayout = pipelineLayout_;

        pipeline_ = std::make_unique<Pipeline>(device_, "../shaders/triangle.vert.spv", "../shaders/triangle.frag.spv", pipelineConfigInfo);
    }

    void Application::recordCommandBuffer(int imageIndex) {

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffers_[imageIndex], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain_->getRenderPass();
        renderPassInfo.framebuffer = swapChain_->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain_->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers_[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChain_->getSwapChainExtent().width;
        viewport.height = (float)swapChain_->getSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain_->getSwapChainExtent();

        vkCmdSetViewport(commandBuffers_[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers_[imageIndex], 0, 1, &scissor);

        renderGameObjects(commandBuffers_[imageIndex]);

        vkCmdEndRenderPass(commandBuffers_[imageIndex]);

        if (vkEndCommandBuffer(commandBuffers_[imageIndex]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void Application::createCommandBuffers()
    {
        commandBuffers_.resize(swapChain_->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = device_.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

        if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Application::freeCommandBuffers() {
        vkFreeCommandBuffers(device_.device(), device_.getCommandPool(), static_cast<uint32_t>(commandBuffers_.size()), commandBuffers_.data());
        commandBuffers_.clear();
    }

    void Application::drawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain_->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        recordCommandBuffer(imageIndex);

        result = swapChain_->submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.wasWindowResized()) {
            window_.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

    void Application::recreateSwapChain() {
        auto extent = window_.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            glfwWaitEvents();
            extent = window_.getExtent();
        }

        vkDeviceWaitIdle(device_.device());

        if (!swapChain_)
            swapChain_ = std::make_unique<SwapChain>(device_, extent);
        else {
            swapChain_ = std::make_unique<SwapChain>(device_, extent, std::move(swapChain_));
            if (swapChain_->imageCount() != commandBuffers_.size()) {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }

        createPipeline();
    }

    void Application::renderGameObjects(VkCommandBuffer commandBuffer) {
        pipeline_->bind(commandBuffer);

        for (auto& object : gameObjects_) { 
            
            PushConstants push{};
            push.offset = object.transform2D_.translation;
            push.color = object.color_;
            push.transform = object.transform2D_.mat2();

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
}