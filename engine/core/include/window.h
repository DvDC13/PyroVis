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
        bool wasWindowResized();
        void resetWindowResizedFlag();

        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        VkExtent2D getExtent() const;

    private:

        GLFWwindow* window_;
        int width_;
        int height_;
        bool framebufferResized_;
        std::string windowName_;

        static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
        void init();
    };
}