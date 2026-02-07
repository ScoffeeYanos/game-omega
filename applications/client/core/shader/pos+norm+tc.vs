#version 430
layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_norm;
layout(location = 2) in vec2 in_tc;

out VS_OUT {
    vec4 pos_wc;
    vec3 norm_wc;
    vec2 tc;
} vs_out;

uniform mat4 model;
uniform mat4 model_normal;
uniform mat4 view;
uniform mat4 proj;

void main() {
    vs_out.pos_wc = model * vec4(in_pos, 1.0);
    vs_out.norm_wc = normalize(mat3(model_normal) * in_norm);
    vs_out.tc = in_tc;
    gl_Position = proj * view * vs_out.pos_wc;
}
