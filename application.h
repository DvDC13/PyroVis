#pragma once

#include "window.h"
#include "pipeline.h"
#include "device.h"
#include "swap_chain.h"

#include <memory>
#include <array>

#define WIDTH 800
#define HEIGHT 600

namespace Pyro
{
    class Application
    {
    public:
        Application();
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(Application&&) = delete;

        void run();
        
    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        Window window_{WIDTH, HEIGHT, "PyroVis"};
        Device device_{window_};
        SwapChain swapChain_{device_, window_.getExtent()};
        std::unique_ptr<Pipeline> pipeline_;
        VkPipelineLayout pipelineLayout_;
        std::vector<VkCommandBuffer> commandBuffers_;
    };
}