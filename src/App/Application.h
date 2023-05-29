#pragma once

#include <exception>
#include <chrono>
#include <thread>

#include "RendererOpenGL.h"
#include "GameplayManager.h"
#include "Input.h"
#include "TestScene.h"
#include "DebugGui.h"
#include "Clock.h"

#include "steam/steam_api.h"
#include "steam/steam_api_common.h"
#include "SteamLobby.h"
#include "GLFW/glfw3.h"

namespace sng {
class DebugGui;
class Scene;

class Application {
public:
	static void init(int argc, char** argv);
	static void shutdown();
	static void updateLoop();

	static Scene& GetCurrentScene() { return *m_scene; };
private:
	static GLFWwindow* m_window;
	static Renderer* m_renderer;
	static Input* m_input;
	static Scene* m_scene;
	static DebugGui* m_gui;
	static SteamLobby* m_steamLobby;
};

}