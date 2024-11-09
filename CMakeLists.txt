cmake_minimum_required(VERSION 3.28.3)
project(PyroVis VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine/utils)

add_library(engine STATIC)
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/engine)
add_subdirectory(engine)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine/core/include)

add_executable(pyroVis main.cpp)

target_link_libraries(pyroVis engine ${Vulkan_LIBRARIES} glfw glm::glm)