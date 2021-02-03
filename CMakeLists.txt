cmake_minimum_required(VERSION 3.17)
project(Application)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/Glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(src)
