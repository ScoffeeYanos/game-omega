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
private:
    glm::ivec2 pos_;
    Storage& storage_;
    static constexpr int radius = 1;
    std::vector<Entity> tiles_;
};
