#version 330 core

layout(location=0) in vec4 position;
layout(location=1) in vec2 texCoords;

out vec2 v_TexCoords;

uniform mat4 projection = mat4(1.0);
uniform vec2 scale = vec2(2.0, 2.0);
uniform vec2 offset = vec2(-1.0, -1.0);

void main() {
    mat4 model = mat4(
        scale.x, 0.0, 0.0, 0.0,
        0.0, scale.y, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        offset.x, offset.y, 0.0, 1.0
    );

    v_TexCoords = texCoords;
    gl_Position = projection * model * position;
}
