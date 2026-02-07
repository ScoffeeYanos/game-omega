#version 430 core

#include "types.fs"

in VS_OUT {
    vec4 pos_wc;
    vec3 norm_wc;
    vec2 tc;
} fs_in;

layout(location = 0) out vec3 out_col;
layout(location = 1) out vec3 out_pos;
layout(location = 2) out vec3 out_norm;

uniform sampler2D diffuse;
uniform Camera cam;

void main() {
    vec3 diff = texture(diffuse, fs_in.tc).rgb;

    out_col = diff;
    out_pos = fs_in.pos_wc.xyz;
    out_norm = fs_in.norm_wc;
}
