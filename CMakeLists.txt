cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 23)

set(project "Sui-Shiijii")
project(${project})

find_package(OpenGL REQUIRED)

add_executable(${project} src/main.cpp src/glad.c src/glad.h)
target_link_libraries(${project} glfw)

