#include "window.h"

namespace Pyro {
    Window::Window(int width, int height, std::string windowName)
    : width_(width), height_(height), windowName_(windowName)
    {
        init();
    }

    Window::~Window() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    bool Window::isClosed() {
        return glfwWindowShouldClose(window_);
    }

    void Window::init() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window_ = glfwCreateWindow(width_, height_, windowName_.c_str(), nullptr, nullptr);
    }
}