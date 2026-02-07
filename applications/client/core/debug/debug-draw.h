#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace DebugDraw {

void init();

void draw();

void clear();

void line(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color = glm::vec4(1, 1, 1, 1),
          float thickness = 1.0);

void ray(const glm::vec3& origin, const glm::vec3& dir, float length, const glm::vec4& color = glm::vec4(1, 1, 1, 1),
         float thickness = 1.0);

void circle(const glm::vec3& center, const glm::vec3& normal, float radius,
            const glm::vec4& color = glm::vec4(1, 1, 1, 1), float thickness = 1.0, int segments = 32);

void sphere(const glm::vec3& center, float radius, const glm::vec4& color = glm::vec4(1, 1, 1, 1),
            float thickness = 1.0, int segments = 16);

void box(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color = glm::vec4(1, 1, 1, 1),
         float thickness = 1.0);

void aabb(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec4& color = glm::vec4(1, 1, 1, 1),
          float thickness = 1.0);

void flat_polygon(const std::vector<glm::vec2>& vertices, float height = 0,
                  const glm::vec4& color = glm::vec4(1, 1, 1, 1), float thickness = 1.0);
} // namespace DebugDraw
