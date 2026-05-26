in vec2 v_uv;

out vec4 frag_color;

uniform vec2  u_center[32];
uniform float u_radius[32];

uniform int   u_count;

uniform vec4  u_fill_color;
uniform vec4  u_outline_color;

uniform float u_threshold;
uniform float u_thickness;

uniform bool  u_outline;

void main()
{
    float field = 0.0;

    for (int i = 0; i < u_count; i++)
    {
        vec2  delta = v_uv - u_center[i];

        float dist2 = dot(delta, delta);

        field += (u_radius[i] * u_radius[i]) / dist2;
    }

    // preenchido
    if (!u_outline)
    {
        if (field < u_threshold)
            discard;

        frag_color = u_fill_color;
        return;
    }

    // contorno
    float outer = u_threshold;
    float inner = outer + u_thickness;

    if (field < outer || field > inner)
        discard;

    frag_color = u_outline_color;
}