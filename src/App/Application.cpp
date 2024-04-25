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
	
	m_window = glfwCreateWindow(800, 450, "SimpleNetGame.exe", nullptr, nullptr);
	if (!m_window) throw std::exception("Problem creating GLFW window!");

	glfwSetFramebufferSizeCallback(m_window, RendererOpenGL::resizeCallback);
    glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, 0);

	if (SteamAPI_RestartAppIfNecessary(480)) { // Replace with your App ID
        std::cerr << "Steam requested restart, may need to include app id file in directory." << std::endl;
        return;
    }

	if (!SteamAPI_Init())
		throw std::exception("Problem initializing Steam");

	m_input = new Input();
	m_input->init(m_window);

	m_renderer = new RendererOpenGL(m_window);

    m_scene = new Scene();
    m_scene->setRenderer(m_renderer);
    m_scene->setInput(m_input);
    m_scene->init();

	m_steamLobby = new SteamLobby();
	m_gui = new DebugGui(m_window, m_steamLobby);
    m_gui->setScene(m_scene);
}

void Application::shutdown() {
	glfwTerminate();
}

void Application::updateLoop() {
    const int targetFPS = 60;
    unsigned short ticks = 0;

    using clock = std::chrono::steady_clock;
    auto next_frame = clock::now();

    while (!glfwWindowShouldClose(m_window)) {
        int delta = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::microseconds(1000000 / targetFPS)).count();
        next_frame += std::chrono::microseconds(1000000 / targetFPS);

		glfwPollEvents();
		m_scene->update(delta);
		m_gui->update();
		glfwSwapBuffers(m_window);

        /*
        if (!(ticks % 10))
            SteamAPI_RunCallbacks();

        ticks++;
         */

        std::this_thread::sleep_until(next_frame);
	}
}

}