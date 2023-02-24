#pragma once

#include "GLFW/glfw3.h"

namespace sng {

class Input {
public:
	void init(GLFWwindow* window);
	void update();
	int getInputs();
private:
	int m_inputs;

	GLFWwindow* m_window;
};

}