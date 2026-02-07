#version 430

#include "types.fs"

in vec2 tc;

out vec3 out_col;

uniform Camera cam;
uniform Geom geom;

uniform PointLight[10] p_lights;
uniform int p_light_count;
uniform DirectionalLight[2] d_lights;
uniform int d_light_count;

uniform Shadow shadow_near;
uniform Shadow shadow_far;
uniform float shadow_intensity;

// SSAO uniforms
uniform sampler2D ssao;
uniform float ssao_intensity;

float sample_shadow(in sampler2D tex, in vec3 c) {
    if (c.z >= 1.0) {
        return 1.0;
    }

    return c.z > texture(tex, c.xy).r ? 0.0 : 1.0; 
}

float get_shadow(in vec3 pos) {
    vec4 clip = (shadow_near.proj * shadow_near.view * vec4(pos, 1));
    vec3 ndc = (clip.xyz / clip.w) * 0.5 + 0.5;

    if (ndc.x >= 0.0 && ndc.x <= 1 && ndc.y >= 0.0 && ndc.y < 1.0) {
        vec2 shadow_texel_size = 1.0 / textureSize(shadow_near.depth, 0).xy;

        float shadow_val = 0.0;
        for (float x = -1.0; x <= 1.0; x++) {
            for (float y = -1.0; y <= 1.0; y++) {
                vec3 sample_pos = ndc + vec3(x * shadow_texel_size.x, y * shadow_texel_size.y, -0.001);
                shadow_val += sample_shadow(shadow_near.depth, sample_pos);
            }
        }

        return (shadow_val / 9.0) * shadow_intensity;
    } else {
        clip = (shadow_far.proj * shadow_far.view * vec4(pos, 1));
        ndc = (clip.xyz / clip.w) * 0.5 + 0.5;
        ndc.z -= 0.005;
        return sample_shadow(shadow_far.depth, ndc) * shadow_intensity;
    }
}


void main() {
    float frag_depth = texture(geom.depth, tc).r;
    gl_FragDepth = frag_depth;

    if (frag_depth == 1) {
        discard;
        return;
    }

    vec3 frag_diff = texture(geom.diff, tc).rgb;
    vec3 frag_pos = texture(geom.pos, tc).xyz;
    vec3 frag_norm = normalize(texture(geom.norm, tc).xyz);

    float shadow_val = get_shadow(frag_pos);

    vec3 amb_light = vec3(0);
    vec3 diff_light = vec3(0);

    for(int i = 0; i < p_light_count; i++){
        PointLight p_light = p_lights[i];
        vec3 light_dir = normalize(p_light.pos - frag_pos);
        float dist = clamp(distance(frag_pos, p_light.pos) / p_light.radius, 0.0, 1.0);
        float falloff = max(0, p_light.intensity * (1 - dist) * (1 - dist));
        float n_dot_l = max(0.0, dot(light_dir, frag_norm));

        amb_light += p_light.ambient * falloff;
        diff_light += p_light.diffuse * n_dot_l * falloff;
    }

    for(int i = 0; i < d_light_count; i++){
        DirectionalLight d_light = d_lights[i];
        vec3 light_dir = -normalize(d_light.dir);
        float n_dot_l = max(0.0, dot(light_dir, frag_norm));

        amb_light += d_light.ambient;
        diff_light += d_light.diffuse * n_dot_l;
    }


    float ssao_factor = texture(ssao, tc).r;
    ssao_factor = mix(1.0, ssao_factor, ssao_intensity);

    vec3 total_light = (amb_light * ssao_factor) + diff_light * shadow_val;

    out_col = frag_diff * total_light;
}
