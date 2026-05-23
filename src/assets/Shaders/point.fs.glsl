#version 330 core
in vec2 vTex;
out vec4 frag_color;
uniform sampler2D uTexture;

void main() {
    frag_color = texture(uTexture, vTex);
}