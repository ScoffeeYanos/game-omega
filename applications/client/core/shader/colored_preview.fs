#version 430 core

in VS_OUT {
    vec4 pos_wc;
    vec3 norm_wc;
    vec2 tc;
} fs_in;

uniform vec3 objectColor;
uniform float alpha;

out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(vec3(-0.4, -0.6, -0.4));
    vec3 norm = normalize(fs_in.norm_wc);
    float ambient = 0.4;
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 lighting = (ambient + diff * 0.6) * objectColor;
    lighting = lighting * 1.3;
    FragColor = vec4(lighting, alpha);
}
