layout(location = 0) in vec2 a_pos;

out vec2 v_uv;

uniform vec2 u_center;
uniform vec2 u_radius;

void main()
{
    v_uv = a_pos;

    vec2 pos = u_center + (a_pos * u_radius);

    gl_Position = vec4(pos, 0.0, 1.0);
}