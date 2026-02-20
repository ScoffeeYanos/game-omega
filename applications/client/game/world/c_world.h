#pragma once
#include <cstddef>

#include "../../../common/game/world/chunk.h"
#include "ecs/storage.h"
class Renderer;
class C_World
{
public:
    C_World();
    void submit(Renderer& renderer);
    static constexpr std::size_t kWorldWidth =10;
    static constexpr std::size_t kWorldHeight=10;
    static constexpr std::size_t kChunkCount =kWorldWidth*kWorldHeight;
private:
    Storage                        world_storage_{};
    std::vector<Chunk>             chunks_;
};
extern C_World* the_world;
