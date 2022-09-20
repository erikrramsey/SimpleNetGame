#pragma once

#include <iostream>

#include "Renderer.h"
#include "GLHeaders.h"

const float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
};
const unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

namespace sng {
class RendererOpenGL : public Renderer {
public:
	RendererOpenGL(GLFWwindow* window);
	virtual void draw();

	static void resizeCallback(GLFWwindow* window, int width, int height);
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	GLuint VBO, VAO, EBO;

};
}