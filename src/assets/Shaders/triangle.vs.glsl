layout(location = 0) in vec2 a_pos;

out vec3 v_bary;

uniform vec2 u_point[3];

void main() {
    if (gl_VertexID == 0)
        v_bary = vec3(1.0, 0.0, 0.0);

    else if (gl_VertexID == 1)
        v_bary = vec3(0.0, 1.0, 0.0);

    else
        v_bary = vec3(0.0, 0.0, 1.0);
    gl_Position = vec4(u_point[gl_VertexID], 0.0, 1.0);
}