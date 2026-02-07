// #include "hexcoords.h"
//
// #include <cmath>
//
// glm::ivec2 util::axial_round(float q, float r)
// {
//     float x = q;
//     float z = r;
//     float y = -x - z;
//
//     float rx = std::round(x);
//     float ry = std::round(y);
//     float rz = std::round(z);
//
//     float x_diff = std::abs(rx - x);
//     float y_diff = std::abs(ry - y);
//     float z_diff = std::abs(rz - z);
//
//     if (x_diff > y_diff && x_diff > z_diff)
//     {
//         rx = -ry - rz;
//     }
//     else if (y_diff > z_diff)
//     {
//         ry = -rx - rz;
//     }
//     else
//     {
//         rz = -rx - ry;
//     }
//
//     return glm::ivec2{static_cast<int>(rx), static_cast<int>(rz)};
// }
//
// glm::vec3 util::world_position_from_axial(int q, int r)
// {
//     constexpr float kHexSize = 1.0f;
//     float world_x = kHexSize * std::sqrt(3.0f) * (static_cast<float>(q) + static_cast<float>(r) * 0.5f);
//     float world_z = kHexSize * 1.5f * static_cast<float>(r);
//     return glm::vec3(world_x, 0.0f, world_z);
// }
//
// glm::vec3 util::world_position_from_axial(glm::ivec2 axial)
// {
//     return world_position_from_axial(axial.x, axial.y);
// }
//
// glm::ivec2 util::axial_from_world_position(const glm::vec3& world)
// {
//     constexpr float kHexSize = 1.0f;
//
//     float r = world.z / (1.5f * kHexSize);
//     float q = (world.x / (std::sqrt(3.0f) * kHexSize)) - r * 0.5f;
//
//     return axial_round(q, r);
// }
//
// glm::ivec2 util::axial_from_world(glm::vec3 world)
// {
//     return axial_from_world_position(world);
// }