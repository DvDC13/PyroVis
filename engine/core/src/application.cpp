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

    std::unique_ptr<IndexBuffer> createCubeIndexBuffer(Device& device, const VertexBuffer& vertexBuffer) {

        std::vector<uint32_t> indices {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9, 
            12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<IndexBuffer>(device, vertexBuffer, indices);
    }

    std::unique_ptr<VertexBuffer> createCube(Device& device, glm::vec3 offset) {
        
        std::vector<Vertex> vertices {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };

        for (auto& v : vertices) {
            v.position += offset;
        }

        return std::make_unique<VertexBuffer>(device, vertices);
    }

    void Application::run()
    {
        RendererSystem rendererSystem(device_, renderer_.getSwapChainRenderPass());

        Camera camera{};
        camera.lookAtDirection(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 1.0f));

        auto viewerObject = GameObject::create();
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
        std::shared_ptr<VertexBuffer> vertexBuffer = createCube(device_, glm::vec3(0.0f, 0.0f, 0.0f));
        std::shared_ptr<IndexBuffer> indexBuffer = createCubeIndexBuffer(device_, *vertexBuffer);

        auto cube = GameObject::create();
        cube.vertexBuffer_ = vertexBuffer;
        cube.indexBuffer_ = indexBuffer;
        cube.transform_.translation = glm::vec3(0.0f, 0.0f, 2.5f);
        cube.transform_.scale = glm::vec3(0.5f, 0.5f, 0.5f);

        gameObjects_.push_back(std::move(cube));
    }
}