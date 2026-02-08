#include "hexcoords.h"

#include <cmath>

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
    constexpr float size = 1.0f;

    float q = static_cast<float>(coord.x);
    float r = static_cast<float>(coord.y);

    float world_x = size * std::sqrt(3.0f) * (q + 0.5f * r);
    float world_z = size * 1.5f * r;

    return glm::vec3(world_x, 0.0f, world_z);
}
glm::ivec2 util::hexcoord::axial_from_world_position(glm::vec3 world)
{
    constexpr float size = 1.0f;

    float r = world.z / (1.5f * size);
    float q = (world.x / (std::sqrt(3.0f) * size)) - 0.5f * r;

    return round({q, r});
}