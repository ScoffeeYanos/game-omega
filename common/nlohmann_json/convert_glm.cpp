#include "convert_glm.hpp"

#include <glm/vec2.hpp>

#include "json.hpp"

void nlohmann::adl_serializer<glm::ivec2>::to_json(json& j, const glm::ivec2& vec) {
	j = json::array({ vec.x, vec.y });
}

void nlohmann::adl_serializer<glm::ivec2>::from_json(const json& j, glm::ivec2& vec) {
	int x = j.at(0).get<int>();
	int y = j.at(1).get<int>();
	vec.x = x;
	vec.y = y;
}
