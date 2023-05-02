cmake_minimum_required(VERSION 3.0)
project(stb_image)

if(WIN32)
    set(BUILD_SHARED_LIBS TRUE)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

add_library(stb_image SHARED include/stb_image.hpp src/stb_image.cpp)
target_include_directories(stb_image PUBLIC include/)