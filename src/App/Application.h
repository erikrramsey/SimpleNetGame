#pragma once

#include <exception>

#include "RendererOpenGL.h"
#include "GameplayManager.h"
#include "Input.h"
#include "Scene.h"
#include "DebugGui.h"
#include "SteamLobby.h"
#include "steam/steam_api.h"
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
	Scene* m_scene;
	DebugGui* m_gui;
	SteamLobby* m_steamLobby;
};

}