#include "primitives.h"

#include <cppgl.h>

using namespace cppgl;

namespace Primitives {

struct Vertex {
    glm::vec3 pos;
    glm::vec4 color;
};

static Shader shader;

static GLuint gVAO = 0;
static GLuint gVBO = 0;

static std::vector<Vertex> gVertices;

void init() {
    shader = Shader{"debug-line", "shader/debug-line.vs", "shader/debug-line.fs"};

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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader->bind();
    shader->uniform("view", current_camera()->view);
    shader->uniform("proj", current_camera()->proj);

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);

    glBufferSubData(GL_ARRAY_BUFFER, 0, gVertices.size() * sizeof(Vertex), gVertices.data());

    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)gVertices.size());

    glBindVertexArray(0);
    shader->unbind();

    glDisable(GL_BLEND);

    gVertices.clear();
}

void circle(const glm::vec3& pos, float radius, const glm::vec4& color, int segements) {
    if (segements < 3)
        return; // need at least 3 for a triangle

    gVertices.push_back({pos, color}); // center

    for (int i = 0; i < segements; ++i) {
        float angle = -2.0f * acos(-1.0f) * i / segements;
        glm::vec3 p = pos + radius * glm::vec3(cos(angle), 0.0f, sin(angle));
        gVertices.push_back({p, color});
        if ((i + 1 < segements)) {
            gVertices.push_back({pos, color});
        }
    }

    gVertices.push_back({pos + radius * glm::vec3{1.0, 0.0, 0.0}, color});
}
} // namespace Primitives