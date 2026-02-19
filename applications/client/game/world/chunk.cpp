#include "chunk.h"

#include "tile.h"
#include "game/util/hexcoords.h"

namespace
{
    constexpr int hex_tile_count_for_radius(const int radius)
    {
        return 1 + (3 * radius * (radius + 1));
    }
}
Chunk::Chunk(const glm::ivec2 pos,Storage& storage):pos_(pos),storage_(storage)
{
    rebuild_tiles();
}
void Chunk::rebuild_tiles()
{
    for (const Entity tile_entity : tiles_)
    {
        storage_.destroy_entity(tile_entity);
    }
    tiles_.clear();
    tiles_.reserve(hex_tile_count_for_radius(radius));
    for(int z = radius; z >= -radius; --z)
    {
        const int x_min = std::max(-radius,-z - radius);
        const int x_max = std::min(radius,-z + radius);
        for (int x = x_min; x <= x_max; ++x)
        {
            const int y = -x - z;
            const Entity tile_entity = storage_.create_entity();
            storage_.emplace<T_Pos>(tile_entity,T_Pos{pos_+glm::ivec2(x,y)});
            storage_.emplace<T_Terrain_Type>(tile_entity,T_Terrain_Type::Grass);
            std::cout << pos_.x+x << " " << pos_.y+y << std::endl;
            tiles_.push_back(tile_entity);
        }
    }
}

void Chunk::submit(Renderer& renderer) const
{
    static Model tile_model{"data/tile.obj"};

    std::vector<glm::mat4> tile_transforms;
    tile_transforms.reserve(tiles_.size());

    for (const Entity tile_entity : tiles_)
    {
        const T_Pos& pos = storage_.get<T_Pos>(tile_entity);
        const glm::vec3 world_pos = util::hexcoord::world_position_from_axial(pos.pos);
        tile_transforms.push_back(glm::translate(glm::mat4{1.0f}, world_pos));
    }

    renderer.submit_model_batch(&tile_model, tile_transforms);
}
