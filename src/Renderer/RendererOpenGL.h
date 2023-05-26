#pragma once

#include <iostream>

#include "Renderer.h"
#include "GLHeaders.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const float quadVerts[] = {
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
	explicit RendererOpenGL(GLFWwindow* window);

	void begin() override;
	void end() override;
	void drawQuad(glm::vec2& pos) override;
    void setClearColor(const glm::vec4& color) override;

	static void resizeCallback(GLFWwindow* window, int width, int height);
private:
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint shaderProgram;

	GLuint VBO, VAO, EBO;
	GLuint transLoc, projLoc;

};
}