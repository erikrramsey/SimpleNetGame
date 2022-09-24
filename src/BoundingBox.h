#pragma once

#include <glm/glm.hpp>

namespace sng {

struct BoundingBox {
	glm::vec2 bl;
	glm::vec2 tr;
};
}