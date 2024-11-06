cmake_minimum_required(VERSION 3.28.3)
project(PyroVis VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(pyroVis main.cpp window.cpp application.cpp pipeline.cpp device.cpp swap_chain.cpp)

target_link_libraries(pyroVis PRIVATE Vulkan::Vulkan glfw)