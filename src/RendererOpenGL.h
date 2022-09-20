#pragma once

#include "Renderer.h"
#include "GLHeaders.h"

namespace sng {
class RendererOpenGL : public Renderer {
public:
	RendererOpenGL(GLFWwindow* window);
	virtual void draw();
};
}