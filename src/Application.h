#pragma once

#include <exception>

#include "RendererOpenGL.h"
#include "NetworkSessionGGPO.h"
#include "GameplayManager.h"
#include "Input.h"

#include "GLFW/glfw3.h"

namespace sng {
class Application {
public:
	Application();
	~Application();

	void init(int argc, char** argv);

	void updateLoop();
private:
	GLFWwindow* m_window;
	Renderer* m_renderer;
	Input* m_input;
	NetworkSession* m_session;
	GameplayManager* m_gameplay;

};

}