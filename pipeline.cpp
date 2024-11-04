#include "pipeline.h"

namespace Pyro
{
    Pipeline::Pipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        createGraphicsPipeline(vertexShaderPath, fragmentShaderPath);
    }

    void Pipeline::createGraphicsPipeline(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
    {
        auto vertexCode = readFile(vertexShaderPath);
        auto fragmentCode = readFile(fragmentShaderPath);

        std::cout << "Vertex shader size: " << vertexCode.size() << '\n';
        std::cout << "Fragment shader size: " << fragmentCode.size() << '\n';
    }

    std::vector<char> Pipeline::readFile(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if(!file.is_open())
        {
            throw std::runtime_error("failed to open file!");
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();
        return buffer;
    }    
}