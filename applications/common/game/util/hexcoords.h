#pragma once

#include <glm/glm.hpp>

namespace util::hexcoord
{
    /// @brief rounds fvec2 position to ivec2.
    glm::ivec2 round(glm::fvec2 coord);
    /// @brief appends z coordinate z=-x-y.
    glm::ivec3 appendz(glm::ivec2 vec2);
    glm::vec3  world_position_from_axial(glm::ivec2 axial);
    glm::ivec2 axial_from_world_position(glm::vec3 world);
}
