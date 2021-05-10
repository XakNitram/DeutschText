# Handle exporting font files.
set(FONT_SRCS

    # List font files to export below.
    Deutsch.ttf
    LICENSE
)

set(FONT_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Data/Fonts/")
list(TRANSFORM FONT_SRCS PREPEND ${FONT_OUTPUT_DIRECTORY} OUTPUT_VARIABLE FONTS_OUT)
list(TRANSFORM FONT_SRCS PREPEND "${CMAKE_SOURCE_DIR}/res/Fonts/" OUTPUT_VARIABLE FONTS_IN)

add_custom_target(fonts ALL DEPENDS ${FONTS_OUT})
add_dependencies(deutsch-text fonts)

add_custom_command(
    DEPENDS ${FONTS_IN}
    OUTPUT ${FONTS_OUT}

    # Make the output directory to put font files into.
    COMMAND ${CMAKE_COMMAND} -E make_directory ${FONT_OUTPUT_DIRECTORY}

    # Copy the font files to the output directory.
    COMMAND ${CMAKE_COMMAND} -E copy ${FONTS_IN} ${FONT_OUTPUT_DIRECTORY}

    # A nice output message to show whether this command had to be run.
    COMMENT "Copied fonts to output"
)
