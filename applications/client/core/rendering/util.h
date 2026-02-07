#pragma once

#include "light.h"
#include <cppgl.h>

namespace rendering::util {
inline void clamp_to_edge(const cppgl::Texture2D& tex) {
    glTextureParameteri(tex->id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(tex->id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
inline void linear_filter(const cppgl::Texture2D& tex) {
    glTextureParameteri(tex->id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(tex->id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

inline void setup_camera(const cppgl::Shader& shader, const cppgl::Camera& cam) {
    shader->uniform("cam.near", cam->near);
    shader->uniform("cam.far", cam->far);
    shader->uniform("cam.pos", cam->pos);
    shader->uniform("cam.dir", cam->dir);
}

inline void quad_with_depth() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    cppgl::Quad::draw();
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
}

} // namespace rendering::util