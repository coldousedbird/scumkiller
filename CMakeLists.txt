cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

project(scumkiller)

set(CMAKE_CXX_STANDART - 20)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/renderer/shaderProgram.cpp
    src/renderer/shaderProgram.hpp)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)
