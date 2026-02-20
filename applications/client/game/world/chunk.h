#pragma once
#include <cstddef>
#include <vector>

#include "core/rendering/renderer.h"
#include "ecs/storage.h"
#include "glm/vec2.hpp"
class Chunk
{
public:
    Chunk(glm::ivec2 pos,Storage& storage);
    void rebuild_tiles();
    void submit(Renderer& renderer) const;
    static constexpr int radius = 5;
private:
    glm::ivec2 pos_;
    Storage& storage_;
    std::vector<Entity> tiles_;
};
