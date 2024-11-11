#pragma once

#include "window.h"
#include "device.h"
#include "logger.h"
#include "game_object.h"
#include "renderer.h"
#include "renderer_system.h"

#include <memory>
#include <array>

#include <glm/glm.hpp>

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
        void loadGameObjects();

        Window window_{WIDTH, HEIGHT, "PyroVis"};
        Device device_{window_};
        Renderer renderer_{window_, device_};
        std::vector<GameObject> gameObjects_;
    };
}