#version 430

#include "types.fs"

in vec2 tc;

out vec4 out_col;

uniform Lighting lighting;
uniform sampler2D bloom_tex;
uniform float bloom_intensity;

vec3 tonemapACES(vec3 x)
{
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;
    return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}


void main() {
    float frag_depth = texture(lighting.depth, tc).r;
    gl_FragDepth = frag_depth;

    vec3 frag_col = texture(lighting.color, tc).rgb;
    vec3 bloom_col = texture(bloom_tex, tc).rgb;
    vec3 final_col = frag_col + bloom_col * bloom_intensity;

    out_col = vec4(tonemapACES(final_col), 1.0f);
}

