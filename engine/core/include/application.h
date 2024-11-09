#pragma once

#include "window.h"
#include "pipeline.h"
#include "device.h"
#include "swap_chain.h"
#include "logger.h"

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
        void loadVertexBuffer();
        void createPipelineLayout();
        void createPipeline();
        void recordCommandBuffer(int imageIndex);
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();

        Window window_{WIDTH, HEIGHT, "PyroVis"};
        Device device_{window_};
        std::unique_ptr<SwapChain> swapChain_;
        std::unique_ptr<Pipeline> pipeline_;
        VkPipelineLayout pipelineLayout_;
        std::vector<VkCommandBuffer> commandBuffers_;
        std::unique_ptr<VertexBuffer> vertexBuffer_;
    };
}