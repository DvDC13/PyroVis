#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

namespace Pyro
{
    class Window
    {
    public:
        Window(int width, int height, std::string windowName);
        ~Window();

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        bool isClosed();

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        VkExtent2D getExtent() const;

    private:
        GLFWwindow* window_;
        const int width_;
        const int height_;
        std::string windowName_;

        void init();
    };
}