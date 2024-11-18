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
        std::vector<std::unique_ptr<Buffer>> uniformBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < SwapChain::MAX_FRAMES_IN_FLIGHT; i++) {
            uniformBuffers[i] = std::make_unique<Buffer>(device_, sizeof(Ubo), 1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uniformBuffers[i]->map();
        }

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

                int frameIndex = renderer_.getCurrentFrameIndex();

                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                };

                Ubo ubo{};
                ubo.projectionViewMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
                uniformBuffers[frameIndex]->writeToBuffer(&ubo);
                uniformBuffers[frameIndex]->flush();

                renderer_.beginSwapChainRenderPass(commandBuffer);
                rendererSystem.renderGameObjects(frameInfo, gameObjects_);
                renderer_.endSwapChainRenderPass(commandBuffer);
                renderer_.endFrame();
            }
        }

        vkDeviceWaitIdle(device_.device());
    }

    void Application::loadGameObjects()
    {
        auto cupMesh = Mesh::createMeshFromFile(device_, "../assets/models/obj/Cup.obj");
        auto cup = GameObject::createObject();
        cup.mesh_ = cupMesh;
        cup.transform_.translation = glm::vec3(0.0f, 0.0f, 2.5f);
        cup.transform_.scale = glm::vec3(0.2f, 0.2f, 0.2f);
        cup.transform_.rotation.x = glm::pi<float>();

        gameObjects_.push_back(std::move(cup));
    }
}