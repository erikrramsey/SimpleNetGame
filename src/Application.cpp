#include "Application.h"

namespace sng {
Application::Application() {}

Application::~Application() {
	glfwTerminate();

	delete m_gameplay;
	m_gameplay = nullptr;
}

void Application::init(int argc, char** argv) {
	if (!glfwInit()) throw std::exception("Problem initializing GLFW!");
	
	m_window = glfwCreateWindow(1280, 720, "SimpleNetGame.exe", nullptr, nullptr);
	if (!m_window) throw std::exception("Problem creating GLFW window!");

	glfwSetFramebufferSizeCallback(m_window, RendererOpenGL::resizeCallback);

	m_gameplay = new GameplayManager();
	m_input = new Input();
	m_input->init(m_window);
	m_renderer = new RendererOpenGL(m_window);
	m_session = new NetworkSessionGGPO(m_gameplay);

	for (auto i = 1; i < argc; i+= 2) {
		m_session->add_player(argv[i + 1], atoi(argv[i]));
	}

	std::cout << "Starting session" << std::endl;
	m_session->start_session();
}

void Application::updateLoop() {
	while (!glfwWindowShouldClose(m_window)) {
		m_renderer->draw();
		m_input->update();
		m_session->update(m_input->getInputs());
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

}

}