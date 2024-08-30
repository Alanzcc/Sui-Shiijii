cmake_minimum_required(VERSION 3.28)

set(project "Sui-Shiijii")
project(${project})

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

add_executable(${project} src/main.cpp src/glad.c src/glad.h
        lib/shader.cpp
        include/matrix.h
)
target_link_libraries(${project} glfw)

