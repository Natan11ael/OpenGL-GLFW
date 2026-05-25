layout(location = 0) in vec2 a_pos;

uniform vec2 u_pos;

void main() {
    gl_Position = vec4(a_pos + u_pos, 0.0, 1.0);
    gl_PointSize = 1.0;
}