#include "Input.h"

namespace sng {
void Input::init(GLFWwindow* window) {
	m_window = window;
	m_inputs = 0;
}

void Input::update() {
	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS) {
		m_inputs = 1;
	}
	else {
		m_inputs = 0;
	}
}

int Input::getInputs() {
	return m_inputs;
}

}