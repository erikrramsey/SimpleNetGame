#pragma once
#include <string>

#include "GLHeaders.h"
#include "glm/glm.hpp"

#include "Transform.h"
#include "Sprite.h"

namespace sng {

class Renderer {
public:
	virtual ~Renderer() = default;
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void drawQuad(glm::vec2& pos) = 0;

    virtual void loadSprite(const std::string& filename, Sprite& sprite) = 0;
    virtual void drawSprite(Transform& trans, Sprite& sprite) = 0;

    virtual void setClearColor(const glm::vec4& color) = 0;
private:
};

}