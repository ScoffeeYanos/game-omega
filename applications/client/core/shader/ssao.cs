#version 430

#include "types.fs"

layout (local_size_x = 16, local_size_y = 16) in;

layout (r16f, binding = 0) writeonly uniform image2D out_ssao;

uniform Geom geom;
uniform mat4 view;
uniform mat4 proj;
uniform float radius;
uniform int sample_count;
uniform float bias;

const int max_samples = 64;
const float pi = 3.14159265359;

float hash12(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.x + p3.y) * p3.z);
}

float radical_inverse_vdc(uint bits) {
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(out_ssao);
    if (pixel.x >= size.x || pixel.y >= size.y) {
        return;
    }

    vec2 uv = (vec2(pixel) + 0.5) / vec2(size);
    float depth = texture(geom.depth, uv).r;
    if (depth >= 1.0) {
        imageStore(out_ssao, pixel, vec4(1.0));
        return;
    }

    vec3 pworld = texture(geom.pos, uv).xyz;
    vec3 normal = normalize(texture(geom.norm, uv).xyz);

    vec3 up = abs(normal.y) < 0.999 ? vec3(0.0, 1.0, 0.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float seed = hash12(vec2(pixel));
    float seed2 = hash12(vec2(pixel.yx + 17.0));

    float occlusion = 0.0;
    int samples = clamp(sample_count, 1, max_samples);
    for (int i = 0; i < max_samples; ++i) {
        if (i >= samples) {
            break;
        }
        float u = (float(i) + seed) / float(samples);
        float v = fract(radical_inverse_vdc(uint(i)) + seed2);
        float phi = 2.0 * pi * u;
        float cos_theta = v;
        float sin_theta = sqrt(max(0.0, 1.0 - cos_theta * cos_theta));
        vec3 hemisphere_dir = vec3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
        vec3 sample_dir = tbn * hemisphere_dir;
        vec3 sworld = pworld + radius * sample_dir;

        vec4 clip = proj * view * vec4(sworld, 1.0);
        vec3 ndc = clip.xyz / max(clip.w, 1e-5);
        vec2 suv = ndc.xy * 0.5 + 0.5;
        if (any(lessThan(suv, vec2(0.0))) || any(greaterThan(suv, vec2(1.0)))) {
            continue;
        }

        vec3 gworld = texture(geom.pos, suv).xyz;
        vec4 gview = view * vec4(gworld, 1.0);
        vec4 sview = view * vec4(sworld, 1.0);

        float hit = step(sview.z + bias, gview.z);
        float dist = max(distance(gworld, sworld), 1e-4);
        float range = smoothstep(0.0, 1.0, radius / dist);
        float projection = max(dot(normal, normalize(gworld - pworld)), 0.0);

        occlusion += hit * range * projection;
    }

    float visibility = 1.0 - (occlusion / float(samples));
    imageStore(out_ssao, pixel, vec4(clamp(visibility, 0.0, 1.0)));
}