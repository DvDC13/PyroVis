#include "application.h"

namespace Pyro
{

    Application::Application()
    {
        Logger logger;
        logger.log(Logger::LogLevel::INFO, "Application started");

        loadGameObjects();
    }

    Application::~Application() {}

    void Application::run()
    {
        RendererSystem rendererSystem(device_, renderer_.getSwapChainRenderPass());

        while (!window_.isClosed())
        {
            glfwPollEvents();
            
            if (auto commandBuffer = renderer_.beginFrame()) {
                renderer_.beginSwapChainRenderPass(commandBuffer);
                rendererSystem.renderGameObjects(commandBuffer, gameObjects_);
                renderer_.endSwapChainRenderPass(commandBuffer);
                renderer_.endFrame();
            }
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
}