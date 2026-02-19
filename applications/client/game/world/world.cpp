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

        // Radius 1 hex chunk spans 3 axial steps center-to-center when tiled.
        const glm::ivec2 chunk_origin{chunk_x * 3, chunk_y * 3};
        chunks_.emplace_back(chunk_origin, world_storage_);
    }
}

void World::submit(Renderer& renderer)
{
    for (const Chunk& chunk : chunks_)
    {
        chunk.submit(renderer);
    }
}