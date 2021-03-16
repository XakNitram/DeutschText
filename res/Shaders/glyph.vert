#version 330 core

in vec4 position;

uniform vec2 scale;
uniform vec2 offset;

void main() {
    mat4 model = mat4(
        scale.x,  0.0,      0.0, 0.0,
        0.0,      scale.y,  0.0, 0.0,
        0.0,      0.0,      1.0, 0.0,
        offset.x, offset.y, 0.0, 1.0
    );

    gl_Position = model * position;
}
