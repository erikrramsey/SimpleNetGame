#pragma once

#include <iostream>

#include "Renderer.h"
#include "GLHeaders.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const float vertices[] = {
		 10.0f,  10.0f, 
		 10.0f,  0.0f, 
		 0.0f,  0.0f, 
		 0.0f,  10.0f,
};
const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

namespace sng {
class RendererOpenGL : public Renderer {
public:
	RendererOpenGL(GLFWwindow* window);
	virtual void begin();
	virtual void end();
	virtual void drawQuad(glm::vec2& pos) override;

	static void resizeCallback(GLFWwindow* window, int width, int height);
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	GLuint VBO, VAO, EBO;
	GLuint transLoc, projLoc;

};
}