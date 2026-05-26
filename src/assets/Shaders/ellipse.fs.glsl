in vec2 v_uv;
out vec4 frag_color;

uniform vec4  u_fill_color;
uniform vec4  u_outline_color;

uniform float u_thickness;

uniform int   u_outline;

void main()
{
    float d = length(v_uv);
    float outer =  1.0;

    if (d > outer) discard;

    // fill
    if (u_outline == 0)
    {
        frag_color = u_fill_color;
        return;
    }

    // outline
    float inner = outer - u_thickness;

    if (d < inner) discard;    

    frag_color = u_outline_color;
}