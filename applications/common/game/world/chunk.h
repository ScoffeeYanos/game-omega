#pragma once
#include <cstddef>
#include <vector>

#include "ecs/storage.h"
#include "glm/vec2.hpp"
class Chunk
{
public:
    Chunk(glm::ivec2 pos,Storage& storage);
    void rebuild_tiles();
    static constexpr int radius = 5;
    std::vector<Entity> tiles_;
private:
    glm::ivec2 pos_;
    Storage& storage_;
};
