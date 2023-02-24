#pragma once
#include "glm/glm.hpp"

namespace sng {

class Renderer {
public:
	virtual ~Renderer() = default;
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void drawQuad(glm::vec2& pos) = 0;
private:
};

}