// #include "utility.h"
// #include "../world/world.h"
// #include "hexcoords.h"
// #include <glm/gtc/matrix_inverse.hpp>
//
// using namespace cppgl;
// using namespace glm;
//
// float util::readDepthAt(vec2 mousePos) {
//     ivec2 res = Context::resolution();
//
//     int x = int(mousePos.x);
//     int y = int(res.y - mousePos.y);
//
//     x = clamp(x, 0, res.x - 1);
//     y = clamp(y, 0, res.y - 1);
//
//     unsigned int depthInt = 0;
//
//     glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, &depthInt);
//
//     return float(depthInt) / float(std::numeric_limits<unsigned int>::max());
// }
//
// vec3 util::backproject(vec2 mouse, float depth, const mat4& view, const mat4& proj, vec2 resolution) {
//     float x = (2.f * mouse.x) / resolution.x - 1.f;
//     float y = 1.f - (2.f * mouse.y) / resolution.y;
//     float z = 2.f * depth - 1.f;
//
//     vec4 clip(x, y, z, 1.f);
//     vec4 world = inverse(proj * view) * clip;
//
//     return vec3(world) / world.w;
// }
//
// vec3 util::mouse_world_pos() {
//     auto mouse = Context::mouse_pos();
//     float depth = readDepthAt(mouse);
//
//     if (depth >= 0.9999f)
//         return vec3(0);
//
//     return backproject(mouse, depth, current_camera()->view, current_camera()->proj, Context::resolution());
// }
//
// Tile* util::pick_tile() {
//     if (!the_world) {
//         return nullptr;
//     }
//
//     auto mouse = Context::mouse_pos();
//     float depth = readDepthAt(mouse);
//
//     if (depth >= 0.9999f) {
//         return nullptr;
//     }
//
//     vec3 world_pos = backproject(mouse, depth, current_camera()->view, current_camera()->proj, Context::resolution());
//
//     glm::ivec2 axial = util::axial_from_world(world_pos);
//     return the_world->tile_at(axial);
// }