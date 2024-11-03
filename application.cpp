#include "application.h"

namespace Pyro
{
    void Application::run()
    {
        while (!window_.isClosed())
        {
            glfwPollEvents();
        }
        
    }
}