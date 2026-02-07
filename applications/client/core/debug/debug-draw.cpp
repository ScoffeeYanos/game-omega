#include "debug-draw.h"

#include <cppgl.h>

using namespace cppgl;

namespace DebugDraw {
struct Vertex {
    glm::vec3 pos;
    glm::vec4 color;
};

static Shader shader;

static GLuint gVAO = 0;
static GLuint gVBO = 0;

static std::vector<Vertex> gVertices;

// ------------------------------------------------------------

void init() {
    shader = Shader{"debug-line", "core/shader/debug-line.vs", "core/shader/debug-line.fs"};

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, 1024 * 1024, nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glBindVertexArray(0);
}

void draw() {
    if (gVertices.empty())
        return;

    shader->bind();
    shader->uniform("view", current_camera()->view);
    shader->uniform("proj", current_camera()->proj);

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertices.size() * sizeof(Vertex), gVertices.data());

    glDrawArrays(GL_LINES, 0, (GLsizei)gVertices.size());

    glBindVertexArray(0);
    shader->unbind();
}

// ------------------------------------------------------------

void clear() { gVertices.clear(); }
// Primitives
// ------------------------------------------------------------

void line(const glm::vec3& a, const glm::vec3& b, const glm::vec4& color, float thickness) {
    gVertices.push_back({a, color});
    gVertices.push_back({b, color});
    return;
}

void ray(const glm::vec3& origin, const glm::vec3& dir, float length, const glm::vec4& color, float thickness) {
    line(origin, origin + dir * length, color, thickness);
}

static glm::vec3 perpendicular(const glm::vec3& v) {
    return glm::abs(v.x) > 0.5f ? glm::normalize(glm::cross(v, glm::vec3(0, 1, 0)))
                                : glm::normalize(glm::cross(v, glm::vec3(1, 0, 0)));
}

void circle(const glm::vec3& center, const glm::vec3& normal, float radius, const glm::vec4& color, float thickness,
            int segments) {
    glm::vec3 n = glm::normalize(normal);
    glm::vec3 t = perpendicular(n);
    glm::vec3 b = glm::cross(n, t);

    float step = 2.0f * 3.1415926535f / segments;

    for (int i = 0; i < segments; ++i) {
        float a0 = step * i;
        float a1 = step * (i + 1);

        glm::vec3 p0 = center + radius * (std::cos(a0) * t + std::sin(a0) * b);
        glm::vec3 p1 = center + radius * (std::cos(a1) * t + std::sin(a1) * b);

        line(p0, p1, color, thickness);
    }
}

void sphere(const glm::vec3& center, float radius, const glm::vec4& color, float thickness, int segments) {
    circle(center, {1, 0, 0}, radius, color, thickness, segments);
    circle(center, {0, 1, 0}, radius, color, thickness, segments);
    circle(center, {0, 0, 1}, radius, color, thickness, segments);
}

void box(const glm::vec3& min, const glm::vec3& max, const glm::vec4& color, float thickness) {
    glm::vec3 v[8] = {{min.x, min.y, min.z}, {max.x, min.y, min.z}, {max.x, max.y, min.z}, {min.x, max.y, min.z},
                      {min.x, min.y, max.z}, {max.x, min.y, max.z}, {max.x, max.y, max.z}, {min.x, max.y, max.z}};

    int e[12][2] = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {0, 4}, {1, 5}, {2, 6}, {3, 7}};

    for (auto& edge : e)
        line(v[edge[0]], v[edge[1]], color, thickness);
}

void aabb(const glm::vec3& center, const glm::vec3& halfExtents, const glm::vec4& color, float thickness) {
    box(center - halfExtents, center + halfExtents, color, thickness);
}

void flat_polygon(const std::vector<glm::vec2>& vertices, float height, const glm::vec4& color, float thickness) {
    if (vertices.size() < 2)
        return;

    for (size_t i = 0; i < vertices.size(); ++i) {
        size_t next = (i + 1) % vertices.size();
        line({vertices[i].x, height, vertices[i].y}, {vertices[next].x, height, vertices[next].y}, color, thickness);
    }
}
} // namespace DebugDraw
