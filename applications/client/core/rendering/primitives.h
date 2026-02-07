#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Primitives {
void init();
void draw();
void circle(const glm::vec3& pos, float radius, const glm::vec4& color, int segments = 32);
}; // namespace Primitives