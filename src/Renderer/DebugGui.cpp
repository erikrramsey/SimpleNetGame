#include "DebugGui.h"

namespace sng {

DebugGui::DebugGui(GLFWwindow* window, SteamLobby* steamLobby) {
	m_lobby = steamLobby;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

DebugGui::~DebugGui() {

}

void DebugGui::update() {
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool show_demo_window = false;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);


	ImGui::Begin("Debug");
	ImGui::Checkbox("Demo Window", &show_demo_window);

	if (ImGui::Button("Create lobby"))
		m_lobby->CreateLobby(4);

	if (ImGui::Button("Connect")) {
		m_lobby->SetData("GameStart", "true");
	}

	static bool shouldPrintMemberData = true;
	ImGui::Checkbox("Print lobby member data", &shouldPrintMemberData);
	if (shouldPrintMemberData)
		PrintMemberData();

	static bool shouldPrintLobbyData = true;
	ImGui::Checkbox("Print lobby data", &shouldPrintLobbyData);
	if (shouldPrintLobbyData)
		PrintLobbyData();

	ImGui::End();

	// Rendering
	ImGui::Render();
	int display_w, display_h;
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGui::PrintLobbyData() {
	const int bufferSize = 200;
	char* k_buffer = new char[bufferSize];
	char* buffer = new char[bufferSize];
	for (int i = 0; i < SteamMatchmaking()->GetLobbyDataCount(m_lobby->GetId()); i++) {
		SteamMatchmaking()->GetLobbyDataByIndex(m_lobby->GetId(), i, k_buffer, bufferSize, buffer, bufferSize);
		ImGui::Text(k_buffer);
		ImGui::Text(buffer);
	}
	delete[] buffer;
	delete[] k_buffer;
}

void DebugGui::PrintMemberData() {
	CSteamID user = SteamUser()->GetSteamID();
	int num_members = m_lobby->GetNumLobbyMembers();
	for (int i = 0; i < num_members; i++) {
		CSteamID id = m_lobby->GetLobbyMemberByIndex(i);
		const char* name = SteamFriends()->GetFriendPersonaName(id);
		if (id == user) {
			ImGui::Text("You are: ");
			ImGui::SameLine();
		}
		ImGui::Text(name);
		ImGui::Text("IP: ");
		ImGui::SameLine();
		ImGui::Text(m_lobby->GetUserData(id, "IP"));
		ImGui::Text("Local IP: ");
		ImGui::SameLine();
		ImGui::Text(m_lobby->GetUserData(id, "LocalIP"));
		ImGui::Separator();
	}
}

}