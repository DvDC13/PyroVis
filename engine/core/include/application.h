#pragma once

#include "window.h"
#include "device.h"
#include "logger.h"
#include "game_object.h"
#include "renderer.h"
#include "renderer_system.h"
#include "keyboard.h"
#include "mesh.h"
#include "descriptor.h"

#include <chrono>
#include <memory>
#include <array>
 
#include <glm/glm.hpp>

#define WIDTH 800
#define HEIGHT 600

namespace Pyro
{
    struct Ubo
    {
        alignas(16) glm::mat4 projectionViewMatrix{1.0f};
        glm::vec4 ambiantLightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.02f);
        glm::vec3 lightPosition = glm::vec3(-1.0f, -1.0f, -1.0f);
        uint32_t padding;
        alignas(16) glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    };

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
        void loadGameObjects();

        Window window_{WIDTH, HEIGHT, "PyroVis"};
        Device device_{window_};
        Renderer renderer_{window_, device_};
        std::unique_ptr<DescriptorPool> descriptorPool_{};
        std::vector<GameObject> gameObjects_;
    };
}