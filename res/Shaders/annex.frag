#version 330 core

in vec2 v_TexCoords;
out vec4 final;

uniform sampler2D atlas;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(atlas, v_TexCoords).r);
    final = sampled;
}
