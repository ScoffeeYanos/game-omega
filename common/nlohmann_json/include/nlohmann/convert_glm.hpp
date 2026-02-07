#pragma once

#include <glm/vec2.hpp>

#include "json_fwd.hpp"

namespace nlohmann {
    template<>
    struct adl_serializer<glm::ivec2> {
        static void to_json(json&, const glm::ivec2&);
        static void from_json(const json&, glm::ivec2&);
    };
}
