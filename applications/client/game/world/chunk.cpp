#include "chunk.h"

#include "tile.h"
#include "glm/vec2.hpp"

Chunk::Chunk(const glm::ivec2 pos,Storage& storage):pos_(pos),storage_(storage)
{
    rebuild_tiles();
}
void Chunk::rebuild_tiles() const
{
    for(int z = radius; z >= -radius; z--)
    {
        const int x_min = std::max(-radius,-z-radius);
        const int x_max = std::min(radius,-z+radius);
        for (int x = x_min; x <= x_max; x++)
        {
            const int y = -x-z;
            const Entity tile_entity = storage_.create_entity();
            storage_.emplace<T_Pos>(tile_entity,T_Pos{pos_+glm::ivec2(x,y)});
            storage_.emplace<T_Terrain_Type>(tile_entity,T_Terrain_Type::Grass);
        }
    }
}
