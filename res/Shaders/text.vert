#version 330 core

layout(location=0) in vec4 position;
//layout(location=0) in vec2 texCoords;
layout(location=1) in vec2 letterScale;
layout(location=2) in vec2 letterOffset;

out vec2 v_TexCoords;

uniform float stringScale;
uniform vec2 stringOffset;
uniform mat4 projection = mat4(1.0);
uniform samplerBuffer coordinates;

void main() {
    // Convert to individual character space
    mat4 characterModel = mat4(
        letterScale.x, 0.0, 0.0, 0.0,
        0.0, letterScale.y, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        letterOffset.x, letterOffset.y, 0.0, 1.0
    );

    // Convert to string space
    mat4 stringModel = mat4(
        stringScale, 0.0, 0.0, 0.0,
        0.0, stringScale, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        stringOffset.x, stringOffset.y, 0.0, 1.0
    );

//    v_TexCoords = texCoords;
    gl_Position = projection * stringModel * characterModel * position;
    v_TexCoords = texelFetch(coordinates, (gl_InstanceID * 4) + gl_VertexID).rg;
}
