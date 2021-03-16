#version 330 core

in vec4 positions;
in vec2 texCoords;

out vec2 v_TexCoords;

void main() {
    v_TexCoords = texCoords;
    gl_Position = positions;
}
