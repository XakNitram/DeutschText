#version 330 core

in vec2 v_TexCoords;
out vec4 final;

uniform sampler2D atlas;

void main() {
    final = texture(atlas, v_TexCoords);
}
