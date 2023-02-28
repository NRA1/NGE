cmake_minimum_required(VERSION 3.0)
project(glad)

add_library(glad include/glad/gl.h src/gl.c)
add_library(glx include/glad/glx.h src/glx.c)
add_library(wgl include/glad/wgl.h src/wgl.c)
target_include_directories(glad PUBLIC include/)
target_include_directories(glx PUBLIC include/)
target_include_directories(wgl PUBLIC include/)