#include "renderer.h"

namespace Pyro
{
    Renderer::Renderer(Window& window, Device& device)
        : window_(window), device_(device)
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    Renderer::~Renderer()
    {
        freeCommandBuffers();
    }

    VkCommandBuffer Renderer::beginFrame() {
        auto result = swapChain_->acquireNextImage(&currentImageIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        isFrameStarted_ = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        return commandBuffer;
    }
    
    void Renderer::endFrame() {
        auto commandBuffer = getCurrentCommandBuffer();
        
        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = swapChain_->submitCommandBuffers(&commandBuffer, &currentImageIndex_);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window_.wasWindowResized()) {
            window_.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        isFrameStarted_ = false;

        currentFrameIndex_ = (currentFrameIndex_ + 1) % SwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void Renderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer) {

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapChain_->getRenderPass();
        renderPassInfo.framebuffer = swapChain_->getFrameBuffer(currentImageIndex_);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain_->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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

        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    }

    void Renderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer) {
        vkCmdEndRenderPass(commandBuffer);
    }

    void Renderer::recreateSwapChain() {
        auto extent = window_.getExtent();
        while (extent.width == 0 || extent.height == 0) {
            glfwWaitEvents();
            extent = window_.getExtent();
        }

        vkDeviceWaitIdle(device_.device());

        if (!swapChain_)
            swapChain_ = std::make_unique<SwapChain>(device_, extent);
        else {
            std::shared_ptr<SwapChain> oldSwapChain = std::move(swapChain_);
            swapChain_ = std::make_unique<SwapChain>(device_, extent, oldSwapChain);
            if (!oldSwapChain->compareSwapFormats(*swapChain_.get())) {
                throw std::runtime_error("Swap chain image (or depth) format has changed!");
            }
        }
    }

    void Renderer::createCommandBuffers()
    {
        commandBuffers_.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = device_.getCommandPool();
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers_.size());

        if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void Renderer::freeCommandBuffers() {
        vkFreeCommandBuffers(device_.device(), device_.getCommandPool(), static_cast<uint32_t>(commandBuffers_.size()), commandBuffers_.data());
        commandBuffers_.clear();
    }
}