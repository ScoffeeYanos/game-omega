#version 430

layout (local_size_x = 16, local_size_y = 16) in;

layout (r16f, binding = 0) writeonly uniform image2D out_blur;

uniform sampler2D input_tex;
uniform int blur_radius;

const int max_blur_radius = 4;

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(out_blur);
    if (pixel.x >= size.x || pixel.y >= size.y) {
        return;
    }

    vec2 texel = 1.0 / vec2(size);
    int radius = clamp(blur_radius, 0, max_blur_radius);
    float sum = 0.0;
    float count = 0.0;
    for (int y = -max_blur_radius; y <= max_blur_radius; ++y) {
        for (int x = -max_blur_radius; x <= max_blur_radius; ++x) {
            if (abs(x) > radius || abs(y) > radius) {
                continue;
            }
            vec2 uv = (vec2(pixel) + vec2(x, y) + 0.5) * texel;
            uv = clamp(uv, vec2(0.0), vec2(1.0));
            sum += texture(input_tex, uv).r;
            count += 1.0;
        }
    }

    float blurred = sum / max(count, 1.0);
    imageStore(out_blur, pixel, vec4(blurred));
}