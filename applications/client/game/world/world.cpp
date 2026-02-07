#include "world.h"
#include <cstddef>
#include "ecs/storage.h"
World::World()
{
    for(std::size_t index=0;index<tiles_.size();++index)
    {
        const int    x          =static_cast<int>(index%10);
        const int    y          =static_cast<int>(index/10);
        const Entity tile_entity=storage_.create_entity();
        storage_.emplace<T_Pos>(tile_entity,T_Pos{{x,y}});
        storage_.emplace<T_Terrain_Type>(tile_entity,T_Terrain_Type::Grass);
        tiles_[index]=tile_entity;
    }
}
