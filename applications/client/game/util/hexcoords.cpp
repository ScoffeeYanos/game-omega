#include "hexcoords.h"

#include <cmath>

namespace util::hexcoord
{
    constexpr float kTileSize = 1.0f;
}
glm::ivec2 util::hexcoord::round(glm::fvec2 coord)
{
    float x = coord.x;
    float y = coord.y;
    float z = -x - y;

    float rx = std::round(x);
    float ry = std::round(y);
    float rz = std::round(z);

    float x_diff = std::abs(rx - x);
    float y_diff = std::abs(ry - y);
    float z_diff = std::abs(rz - z);

    if (x_diff > y_diff && x_diff > z_diff)
    {
        rx = -ry - rz;
    }
    else if (y_diff > z_diff)
    {
        ry = -rx - rz;
    }
    else
    {
        rz = -rx - ry;
    }

    return glm::ivec2{static_cast<int>(rx), static_cast<int>(ry)};
}
glm::vec3 util::hexcoord::world_position_from_axial(glm::ivec2 coord)
{
    float q = static_cast<float>(coord.x);
    float r = static_cast<float>(coord.y);

    float world_x = kTileSize * 1.5f * q;
    float world_z = kTileSize * std::sqrt(3.0f) * (r - 0.5f * q);

    return glm::vec3(world_x, 0.0f, world_z);
}
glm::ivec2 util::hexcoord::axial_from_world_position(glm::vec3 world)
{
    float q = world.x / (1.5f * kTileSize);
    float r = (world.z / (std::sqrt(3.0f) * kTileSize)) - 0.5f * q;

    return round({q, r});
}