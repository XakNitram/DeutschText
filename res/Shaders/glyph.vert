#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoords;

out vec2 v_TexCoords;

uniform vec2 offset;
uniform vec2 scale;
uniform mat4 projection = mat4(1.0);

void main() {
    v_TexCoords = texCoords;
    mat4 model = mat4(
        scale.x,  0.0,      0.0, 0.0,
        0.0,      scale.y,  0.0, 0.0,
        0.0,      0.0,      1.0, 0.0,
        offset.x, offset.y, 0.0, 1.0
    );

    gl_Position = projection * model * position;
}
