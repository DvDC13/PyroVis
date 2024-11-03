cmake_minimum_required(VERSION 3.28.3)
project(PyroVis VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(pyroVis main.cpp)

target_link_libraries(PyroVis PRIVATE Vulkan::Vulkan glfw)