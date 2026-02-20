#pragma once
#include <cstddef>

#include "common/game/world/chunk.h"
#include "ecs/storage.h"
class Renderer;
class World
{
public:
    World();
    void submit(Renderer& renderer);
    static constexpr std::size_t kWorldWidth =10;
    static constexpr std::size_t kWorldHeight=10;
    static constexpr std::size_t kChunkCount =kWorldWidth*kWorldHeight;
private:
    Storage                        world_storage_{};
    std::vector<Chunk>             chunks_;
};
extern World* the_world;
