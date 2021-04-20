add_executable(
    deutsch-text Application.cpp

    # CORE
    Core/Window.hpp
    Core/Window.cpp
    Core/Event.hpp
    Core/Event.cpp

    # PRIMITIVES
    Primitives/Font.hpp
    Primitives/Font.cpp
    Primitives/Text.hpp
    Primitives/Text.cpp
)

target_include_directories(deutsch-text PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Use precompiled headers.
target_precompile_headers(deutsch-text PRIVATE pch.hpp pch.cpp)

# Link in glad and glfw libraries.
target_link_libraries(deutsch-text PRIVATE OpenGL32)
target_link_libraries(deutsch-text PRIVATE glad)
target_link_libraries(deutsch-text PRIVATE glfw)

# Link in LWVL library.
target_link_libraries(deutsch-text PRIVATE lwvl)

# Link in FreeType library.
target_include_directories(deutsch-text PRIVATE "${CMAKE_SOURCE_DIR}/vendor/FreeType/include")
target_link_libraries(deutsch-text PRIVATE "${CMAKE_SOURCE_DIR}/vendor/FreeType/freetype.lib")

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

# Copy font to output directory.
add_custom_command(
    TARGET deutsch-text POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res/Fonts/
    ${CMAKE_BINARY_DIR}/bin/Data/Fonts/
    COMMENT "Copied font folder to output"
)

#set(
#    shader_sources
#
#    # Shaders
#    text.vert
#    text.frag
#    glyph.vert
#    glyph.frag
#    mazing.frag
#    spacetime.frag
#)

#list(TRANSFORM shader_sources PREPEND ${CMAKE_BINARY_DIR}/res/Shaders/ OUTPUT_VARIABLE shader_inputs)
#list(TRANSFORM shader_sources PREPEND ${CMAKE_BINARY_DIR}/bin/Data/Shaders/ OUTPUT_VARIABLE shader_outputs)

add_custom_target(
    shaders ALL
    DEPENDS
#    ${shader_outputs}
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/text.vert
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/text.frag
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/glyph.vert
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/glyph.frag
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/mazing.frag
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/spacetime.frag
)
add_dependencies(deutsch-text shaders)

# Copy shaders to output directory.
add_custom_command(
    DEPENDS
#    ${shader_inputs}
    ${CMAKE_SOURCE_DIR}/res/Shaders/text.vert
    ${CMAKE_SOURCE_DIR}/res/Shaders/text.frag
    ${CMAKE_SOURCE_DIR}/res/Shaders/glyph.vert
    ${CMAKE_SOURCE_DIR}/res/Shaders/glyph.frag
    ${CMAKE_SOURCE_DIR}/res/Shaders/mazing.frag
    ${CMAKE_SOURCE_DIR}/res/Shaders/spacetime.frag
    OUTPUT
#    ${shader_outputs}
#    "${CMAKE_BINARY_DIR}/bin/Data/Shaders/${shader_sources}"
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/text.vert
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/text.frag
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/glyph.vert
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/glyph.frag
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/mazing.frag
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders/spacetime.frag

    COMMENT "Copied shader folder to output"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res/Shaders/
    ${CMAKE_BINARY_DIR}/bin/Data/Shaders
)
