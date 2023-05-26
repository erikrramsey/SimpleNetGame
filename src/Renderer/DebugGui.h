#pragma once

#include "Application.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <string>
#include "Scene.h"
#include "Lobject.h"

#include "SteamLobby.h"
#include "steam/isteamfriends.h"

namespace sng {
class SteamLobby;

class DebugGui {
public:
	DebugGui(GLFWwindow* window, SteamLobby* steamLobby);
	~DebugGui();


    void setScene(Scene* scene) { m_scene = scene; }
	void update();
    void displayLobject(Lobject* obj);
    void componentWindow(Lobject* m_rendered);
    void familyComponent();
    void transformComponent();
private:
    Scene* m_scene;
    Lobject* m_selected;
	SteamLobby* m_lobby;

    bool m_renaming = false;

	void PrintLobbyData();
	void PrintMemberData();

    void sceneView();

};

}