target_sources(
    deutsch-text
    PRIVATE
        Application.cpp

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