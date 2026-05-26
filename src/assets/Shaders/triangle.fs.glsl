in vec3  v_bary;
out vec4 frag_color;

uniform vec4  u_fill_color;
uniform vec4  u_outline_color;

uniform float u_thickness;

uniform int   u_outline;

void main() {
    float d = min(min(v_bary.x, v_bary.y), v_bary.z);

    // fill
    if (u_outline == 0)
    {
        frag_color = u_fill_color;
        return;
    }

    if (d > u_thickness)
        discard;

    frag_color = u_outline_color;
}