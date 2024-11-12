#pragma once

#include "window.h"
#include "device.h"
#include "swap_chain.h"

#include <memory>
#include <array>

namespace Pyro
{

    class Renderer
    {
    public:
        Renderer(Window& window, Device& device);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        VkRenderPass getSwapChainRenderPass() const { return swapChain_->getRenderPass(); }

        bool isFrameInProgress() const { return isFrameStarted_; }
        VkCommandBuffer getCurrentCommandBuffer() const { return commandBuffers_[currentFrameIndex_]; }
        
        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        float getAspectRatio() const { return swapChain_->extentAspectRatio(); }

        int getCurrentFrameIndex() const { return currentFrameIndex_; }

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window& window_;
        Device& device_;
        std::unique_ptr<SwapChain> swapChain_;
        std::vector<VkCommandBuffer> commandBuffers_;

        uint32_t currentImageIndex_ = 0;
        bool isFrameStarted_ = false;
        int currentFrameIndex_ = 0;
    };
}