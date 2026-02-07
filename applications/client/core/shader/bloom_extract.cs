#version 430

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba16f, binding = 0) writeonly uniform image2D out_bloom;

uniform sampler2D input_color;
uniform float threshold_low;
uniform float threshold_high;
uniform float contribution_min;
uniform float contribution_max;

vec3 rgb_to_xyz(vec3 rgb) {
    const mat3 rgb_to_xyz_mat = mat3(
        0.4124, 0.3576, 0.1805,
        0.2126, 0.7152, 0.0722,
        0.0193, 0.1192, 0.9505
    );
    return rgb_to_xyz_mat * rgb;
}

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(out_bloom);
    if (pixel.x >= size.x || pixel.y >= size.y) {
        return;
    }

    vec2 uv = (vec2(pixel) + 0.5) / vec2(size);
    vec3 color = texture(input_color, uv).rgb;
    float luminance = rgb_to_xyz(color).y;
    float brightness_factor = smoothstep(threshold_low, threshold_high, luminance);
    float contribution = mix(contribution_min, contribution_max, brightness_factor);
    vec3 bloom_color = color * brightness_factor * contribution;
    imageStore(out_bloom, pixel, vec4(bloom_color, 1.0));
}