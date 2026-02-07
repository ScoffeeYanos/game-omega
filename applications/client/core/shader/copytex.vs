#version 330

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec2 in_tc;

out vec2 tc;

void main() {
    vec4 pos = vec4(2.0f * in_pos - 1.0f, 1.0f);
    pos.z = -0.8f;
    gl_Position = pos;
    tc = in_tc;
}
