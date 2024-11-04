#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "device.h"

namespace Pyro
{
    class Pipeline
    {
    public:
        Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&&) = default;
        Pipeline& operator=(Pipeline&&) = default;

    private:
        static std::vector<char> readFile(const std::string& filePath);

        void createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    };
}