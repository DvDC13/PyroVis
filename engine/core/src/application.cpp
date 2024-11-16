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

        Camera camera{};
        camera.lookAtDirection(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 1.0f));

        auto viewerObject = GameObject::createObject();
        Keyboard keyboard{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window_.isClosed())
        {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            if (frameTime > 1.0f / 30.0f) {
                frameTime = 1.0f / 30.0f;
            }

            keyboard.moveInPlaneXZ(window_.getWindow(), frameTime, viewerObject);

            camera.lookAtInYXZ(viewerObject.transform_.translation, viewerObject.transform_.rotation);

            float aspect = renderer_.getAspectRatio();
            //camera.setOrthographicProjection(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
            camera.setPerspectiveProjection(50.0f, aspect, 0.1f, 10.0f);
            
            if (auto commandBuffer = renderer_.beginFrame()) {
                renderer_.beginSwapChainRenderPass(commandBuffer);
                rendererSystem.renderGameObjects(commandBuffer, gameObjects_, camera);
                renderer_.endSwapChainRenderPass(commandBuffer);
                renderer_.endFrame();
            }
        }

        vkDeviceWaitIdle(device_.device());
    }

    void Application::loadGameObjects()
    {
        auto cup = GameObject::createObjectfromFile(device_, "../assets/models/obj/Cup.obj");

        cup.transform_.translation = glm::vec3(0.0f, 0.0f, 2.5f);
        cup.transform_.scale = glm::vec3(0.2f, 0.2f, 0.2f);
        cup.transform_.rotation.x = glm::pi<float>();

        gameObjects_.push_back(std::move(cup));
    }
}