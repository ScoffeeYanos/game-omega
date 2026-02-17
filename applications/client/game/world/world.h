#pragma once
#include <array>
#include <cstddef>
#include "tile.h"
#include "ecs/storage.h"
class Renderer;
class World
{
public:
    World();
    void submit(Renderer& renderer);
    static constexpr std::size_t kWorldWidth =100;
    static constexpr std::size_t kWorldHeight=100;
    static constexpr std::size_t kTileCount  =kWorldWidth*kWorldHeight;
private:
    Storage                       storage_{};
    std::array<Entity,kTileCount> tiles_{};
};
extern World* the_world;
