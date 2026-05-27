in vec2 vTex;
out vec4 frag_color;

uniform sampler2D u_texture;

uniform vec4 u_color;
uniform float u_time;
uniform float u_intensity;

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main() {
    float t = floor(u_time * 10.0) / 10.0; // discretiza o tempo — efeito "travado"

    // deslocamento por linha — simula corrupção de scanline
    float line = floor(vTex.y * 50.0);
    float shift = rand(vec2(line, t)) * u_intensity * 0.05;

    // split RGB — cada canal desloca diferente
    float r = texture(u_texture, vTex + vec2(shift, 0.0)).r;
    float g = texture(u_texture, vTex).g;
    float b = texture(u_texture, vTex - vec2(shift, 0.0)).b;
    float a = texture(u_texture, vTex).a;

    // bloco de corrupção aleatório
    float block = floor(vTex.y * 20.0);
    float corrupt = step(0.95, rand(vec2(block, t)));
    vec2 corruptTex = vTex + vec2(rand(vec2(block, t + 1.0)) * corrupt * u_intensity * 0.1, 0.0);

    vec4 color = vec4(r, g, b, a);
    vec4 corrupt_color = texture(u_texture, corruptTex);
    color = mix(color, corrupt_color, corrupt * u_intensity);

    frag_color = color * u_color;
}