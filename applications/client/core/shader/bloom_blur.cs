#version 430

layout (local_size_x = 16, local_size_y = 16) in;

layout (rgba16f, binding = 0) writeonly uniform image2D out_blur;

uniform sampler2D input_tex;

const float kernel[9] = float[](
    1.0, 2.0, 1.0,
    2.0, 4.0, 2.0,
    1.0, 2.0, 1.0
);

void main() {
    ivec2 pixel = ivec2(gl_GlobalInvocationID.xy);
    ivec2 size = imageSize(out_blur);
    if (pixel.x >= size.x || pixel.y >= size.y) {
        return;
    }

    ivec2 src_size = textureSize(input_tex, 2);
    vec3 sum = vec3(0.0);
    float weight_sum = 0.0;
    int index = 0;
    for (int y = -1; y <= 1; ++y) {
        for (int x = -1; x <= 1; ++x) {
            ivec2 coord = clamp(pixel + ivec2(x, y), ivec2(0), src_size - 1);
            float weight = kernel[index++];
            sum += texelFetch(input_tex, coord, 2).rgb * weight;
            weight_sum += weight;
        }
    }

    vec3 blurred = sum / max(weight_sum, 1e-6);
    imageStore(out_blur, pixel, vec4(blurred, 1.0));
}