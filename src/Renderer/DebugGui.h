#pragma once

#include "Application.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>

#include "SteamLobby.h"
#include "steam/isteamfriends.h"

namespace sng {
class SteamLobby;

class DebugGui {
public:
	DebugGui(GLFWwindow* window, SteamLobby* steamLobby);
	~DebugGui();


	void update();
private:
	SteamLobby* m_lobby;

	void PrintLobbyData();
	void PrintMemberData();
};

}