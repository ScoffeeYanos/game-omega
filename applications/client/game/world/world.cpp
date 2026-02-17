#include "world.h"
#include <cstddef>

#include "core/rendering/model.h"
#include "core/rendering/renderer.h"
#include "ecs/storage.h"
#include "game/util/hexcoords.h"
World::World()
{
    for(std::size_t index=0;index<tiles_.size();++index)
    {
        const int    x          =static_cast<int>(index%kWorldWidth);
        const int    y          =static_cast<int>(index/kWorldWidth);
        const Entity tile_entity=storage_.create_entity();
        storage_.emplace<T_Pos>(tile_entity,T_Pos{{x,y}});
        storage_.emplace<T_Terrain_Type>(tile_entity,T_Terrain_Type::Grass);
        tiles_[index]=tile_entity;
    }
}

void World::submit(Renderer& renderer)
{
    static Model tile_model{"data/tile.obj"};

    storage_.for_each<T_Pos>([&renderer](const T_Pos& pos)
    {
        const glm::vec3 world_pos =
            util::hexcoord::world_position_from_axial(pos.pos);

        const glm::mat4 transform =
            glm::translate(glm::mat4{1.0f}, world_pos);

        renderer.submit_model(&tile_model, transform);
    });
}