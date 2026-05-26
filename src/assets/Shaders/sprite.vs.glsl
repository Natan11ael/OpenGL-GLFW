layout(location = 0) in vec2 a_pos;
layout(location = 1) in vec2 a_tex;

out vec2 vTex;

uniform vec2  u_pos;
uniform vec2  u_scale;

uniform float u_rotation;

uniform vec2  u_tex_scale;

void main() {
    vec2 pos = a_pos * u_scale;

    float c = cos(u_rotation);
    float s = sin(u_rotation);

    pos = vec2(
        pos.x * c - pos.y * s,
        pos.x * s + pos.y * c
    );

    pos += u_pos;

    gl_Position = vec4(pos, 0.0, 1.0);

    vTex = a_tex * u_tex_scale;
}