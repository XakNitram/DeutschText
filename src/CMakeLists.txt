add_executable(
        deutsch-text Application.cpp

        # CORE
        Core/Window.hpp
        Core/Window.cpp
        Core/Event.hpp
        Core/Event.cpp
)

target_include_directories(deutsch-text PRIVATE "${CMAKE_SOURCE_DIR}/src")

# Link in glad and glfw libraries.
target_link_libraries(deutsch-text PRIVATE OpenGL32)
target_link_libraries(deutsch-text PRIVATE glad)
target_link_libraries(deutsch-text PRIVATE glfw)
#target_link_libraries(deutsch-text PRIVATE stb)

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
