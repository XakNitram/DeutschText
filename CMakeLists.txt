cmake_minimum_required(VERSION 3.17)
project(Application)

set(CMAKE_CXX_STANDARD 17)

add_executable(deutsch-text "")

# Mark the project as C++ 17 and set the output directory.
set_target_properties(
    deutsch-text PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO

    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/
)

add_subdirectory(vendor/Glad)
add_subdirectory(vendor/GLFW)
add_subdirectory(src)
add_subdirectory(res)
