#include "Application.h"

namespace sng {

GLFWwindow* Application::m_window;
Renderer* Application::m_renderer;
Input* Application::m_input;
Scene* Application::m_scene;
DebugGui* Application::m_gui;
SteamLobby* Application::m_steamLobby;

void Application::init(int argc, char** argv) {
	if (!glfwInit()) throw std::exception("Problem initializing GLFW!");
	
	m_window = glfwCreateWindow(1280, 720, "SimpleNetGame.exe", nullptr, nullptr);
	if (!m_window) throw std::exception("Problem creating GLFW window!");

	glfwSetFramebufferSizeCallback(m_window, RendererOpenGL::resizeCallback);

	if (SteamAPI_RestartAppIfNecessary(480)) { // Replace with your App ID
        std::cerr << "Steam requested restart, may need to include app id file in directory." << std::endl;
        return;
    }

	if (!SteamAPI_Init())
		throw std::exception("Problem initializing Steam");

	m_input = new Input();
	m_input->init(m_window);
	m_renderer = new RendererOpenGL(m_window);
	m_scene = new Scene(m_input, m_renderer);
	m_steamLobby = new SteamLobby();
	m_gui = new DebugGui(m_window, m_steamLobby);
}

void Application::shutdown() {
	glfwTerminate();
}

void Application::updateLoop() {
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
		m_scene->update(1);
		m_gui->update();
		glfwSwapBuffers(m_window);
		SteamAPI_RunCallbacks();
	}
}

}