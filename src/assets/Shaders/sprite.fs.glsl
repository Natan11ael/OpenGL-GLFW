in vec2 vTex;
out vec4 frag_color;

uniform sampler2D u_texture;

uniform vec4 u_color;

void main() {
    frag_color = texture(u_texture, vTex) * u_color;
}