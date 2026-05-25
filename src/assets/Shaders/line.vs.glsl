layout(location = 0) in vec2 a_pos;

uniform vec2 u_point[2];

void main() {
    gl_Position = vec4(u_point[gl_VertexID], 0.0, 1.0);
}