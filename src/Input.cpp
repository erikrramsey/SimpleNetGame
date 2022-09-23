#include "Input.h"

namespace sng {
void Input::init(GLFWwindow* window) {
	m_window = window;
	m_inputs = 0;
}

void Input::update() {
	m_inputs = 0;
	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)	m_inputs = 1;
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)	m_inputs = -1;
}

int Input::getInputs() {
	return m_inputs;
}

}