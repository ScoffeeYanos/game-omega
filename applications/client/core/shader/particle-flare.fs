#version 430
flat in int time;
uniform vec2 near_far;
uniform vec2 screenres;
uniform sampler2D gbuf_depth;
uniform sampler2D particle_mask;
uniform vec3 particle_color;

out vec4 out_col;

float linear_depth(in float depth, in float near, in float far) { return (2.0 * near) / (far + near - depth * (far - near)); }

void main() {
    vec2 texCoord = gl_PointCoord;
    float maskAlpha = texture(particle_mask, texCoord).r;
    if (maskAlpha < 0.01) {
            discard;
        }

    vec4 col = vec4(particle_color, 1.0);
    float fade_time = clamp(time / 500.f, 0, 1);
    out_col = pow(fade_time, 2) * col;
    vec2 tc = gl_FragCoord.xy / screenres;
    if (gl_FragCoord.z > texture(gbuf_depth,tc).r)
        out_col = vec4(0,1,0,1);
    float frag_depth = linear_depth(gl_FragCoord.z, near_far.x, near_far.y);
    float geom_depth = linear_depth(texture(gbuf_depth, gl_FragCoord.xy / screenres).r, near_far.x, near_far.y);
    float fade_depth = clamp(abs(frag_depth - geom_depth) / 0.001f, 0, 1);
    out_col *= vec4(fade_depth * fade_depth);
}

