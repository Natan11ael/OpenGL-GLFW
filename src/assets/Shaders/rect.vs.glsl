layout(location = 0) in vec2 a_pos;

out vec2 v_uv;

uniform vec2 u_point[4];

void main() {
    v_uv = a_pos;
    gl_Position = vec4(u_point[gl_VertexID], 0.0, 1.0);
}