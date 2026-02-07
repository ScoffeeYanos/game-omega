#version 430

#include "types.fs"

in vec2 tc;

uniform Geom geom;

out vec4 out_col;

void main() {
    float frag_depth = texture(geom.depth, tc).r;
    gl_FragDepth = frag_depth;
    
    if (frag_depth == 1) {
        discard;
        return;
    }

    vec3 frag_diff = texture(geom.diff, tc).rgb;

    out_col = vec4(frag_diff, 1.0);
}