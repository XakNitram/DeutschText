#version 330 core

in vec2 v_TexCoords;
out vec4 final;

uniform sampler2D glyph;

void main() {
    final = vec4(1.0, 1.0, 1.0, texture(glyph, v_TexCoords).r);
}
