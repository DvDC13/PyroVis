#pragma once

#include "window.h"

#define WIDTH 800
#define HEIGHT 600

namespace Pyro
{
    class Application
    {
    public:

        void run();
        
    private:
        Window window_{WIDTH, HEIGHT, "PyroVis"};
    };
}