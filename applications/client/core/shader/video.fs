#version 330

in vec2 tc;

uniform sampler2D frame;

out vec4 out_col;

void main() {
    // Flip vertically during sampling
    vec2 flipped_tc = vec2(tc.x, 1.0 - tc.y);
    out_col = vec4(texture(frame, flipped_tc).rgb, 1.0);
}