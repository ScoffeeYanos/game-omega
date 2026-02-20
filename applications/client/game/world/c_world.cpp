#include "c_world.h"
#include <cstddef>

#include "common/game/world/tile.h"
#include "core/rendering/model.h"
#include "core/rendering/renderer.h"
#include "ecs/storage.h"
#include "common/game/util/hexcoords.h"
C_World::C_World()
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
        static Model tile_model{"data/tile.obj"};

        std::vector<glm::mat4> tile_transforms;
        tile_transforms.reserve(chunk.tiles_.size());

        for (const Entity tile_entity : chunk.tiles_)
        {
            const T_Pos& pos = world_storage_.get<T_Pos>(tile_entity);
            const glm::vec3 world_pos = util::hexcoord::world_position_from_axial(pos.pos);
            tile_transforms.push_back(glm::translate(glm::mat4{1.0f}, world_pos));
        }

        renderer.submit_model_batch(&tile_model, tile_transforms);
    }
}