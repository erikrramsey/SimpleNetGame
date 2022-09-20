#include "RendererOpenGL.h"

namespace sng {


RendererOpenGL::RendererOpenGL(GLFWwindow* window) {
	glfwMakeContextCurrent(window);
	glewInit();

	GLuint v_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(v_shader, 1, &vertex_shader, nullptr);
	glCompileShader(v_shader);
	glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
}

void RendererOpenGL::draw() {
	glClear(GL_COLOR_BUFFER_BIT);
}
}