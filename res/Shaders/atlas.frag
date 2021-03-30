#version 330 core

in vec2 v_TexCoords;

uniform sampler2D atlas;

void main() {
    gl_FragColor = texture(atlas, v_TexCoords);
}
