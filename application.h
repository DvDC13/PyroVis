#pragma once

#include "window.h"
#include "pipeline.h"

#define WIDTH 800
#define HEIGHT 600

namespace Pyro
{
    class Application
    {
    public:
        Application() = default;
        
        void run();
        
    private:
        Window window_{WIDTH, HEIGHT, "PyroVis"};
        Pipeline pipeline_{"../shaders/triangle.vert.spv", "../shaders/triangle.frag.spv"};
    };
}