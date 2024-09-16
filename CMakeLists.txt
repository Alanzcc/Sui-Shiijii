cmake_minimum_required(VERSION 3.28)

set(project "Sui-Shiijii")
project(${project} VERSION 0.1 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bins)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")


add_executable(${project} src/main.cpp src/glad.c src/stb.cpp
)
target_include_directories(${project} PRIVATE include lib)
target_link_libraries(${project} glfw)