#include "world.h"
#include <cstddef>

#include "core/rendering/model.h"
#include "core/rendering/renderer.h"
#include "ecs/storage.h"
#include "game/util/hexcoords.h"
World::World()
{
    chunks_.reserve(kChunkCount);

    for(std::size_t index=0;index<kChunkCount;++index)
    {
        const int chunk_x = static_cast<int>(index % kWorldWidth);
        const int chunk_y = static_cast<int>(index / kWorldWidth);

        constexpr int radius = Chunk::radius;
        const glm::ivec2 chunk_origin{
            chunk_x * (radius + 1) - chunk_y * radius,
            chunk_x * radius + chunk_y * ((2 * radius) + 1)
        };
        chunks_.emplace_back(chunk_origin, world_storage_);
    }
}

void C_World::submit(Renderer& renderer)
{
    for (const Chunk& chunk : chunks_)
    {
        chunk.submit(renderer);
    }
}