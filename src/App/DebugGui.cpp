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

    m_selected = nullptr;
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

    sceneView();
    componentWindow();

	if (ImGui::Button("Create lobby"))
		m_lobby->CreateLobby(4);

	if (ImGui::Button("Connect")) {
		m_lobby->SetData("GameStart", "true");
	}

    auto& scene = Application::GetCurrentScene();
    if (ImGui::Button("Start Solo")) {
        scene.startOfflineSession();
    }

    if (ImGui::Button("Start Synctest")) {
       scene.startSyncTest(new NetworkSessionGGPO());
    }

    static NetworkSessionGGPO* ggpo = nullptr;
    if (ImGui::Button("Create local ggpo session")) {
        ggpo = new NetworkSessionGGPO();
    }

    if (ggpo) {
        static bool enabled[4];
        static bool self[4];

        for (int i = 0; i < 4; i++) {
            ImGui::PushID(i);

            ImGui::Text("700%d", i);
            ImGui::SameLine();
            ImGui::Checkbox("Self", &self[i]);
            ImGui::SameLine();
            ImGui::Checkbox("Enabled", &enabled[i]);

            ImGui::PopID();
            if (!enabled[i]) break;
        }


        if (ImGui::Button("Begin")) {
            for (int i = 0; i < 4; i++) {
                if (!enabled[i]) break;

                if (self[i])
                    ggpo->add_player("local", 7000 + i);
                else
                    ggpo->add_player("127.0.0.1", 7000 + i);
            }

            scene.startOnlineSession(ggpo);
        }
    }

    auto& ss = scene.getSharedState();

    auto& v0 = ss.position[0];
    ImGui::Text("%f %f", v0.x, v0.y);


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

// Steam Lobby ////////////////////////////////////////////////////////////////

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
        ImGui::Text("Port: ");
        ImGui::SameLine();
        ImGui::Text(m_lobby->GetUserData(id, "Port"));
        ImGui::Separator();
	}
}


// Lobjects ///////////////////////////////////////////////////////////////////

void DebugGui::sceneView() {
    ImGui::Begin("Scene");

    if (ImGui::Button("Create LObject")) {
        m_scene->create_Lobject();
    }

    displayLobject(m_scene->get_root());

    ImGui::End();
}

void DebugGui::displayLobject(Lobject* obj) {
    std::string name = std::string(obj->get_name());

    if (m_renaming && obj == m_selected) {
        char input[80] = "";
        strcpy_s(input, m_selected->get_name());
        ImGui::SetKeyboardFocusHere();
        if (ImGui::InputText("##", input, 80, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
            obj->set_name(input);
            m_renaming = false;
        }
        else if (ImGui::IsItemDeactivated()) {
            m_renaming = false;
        }
        return;
    }

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (obj == m_selected) flags |= ImGuiTreeNodeFlags_Selected;
    if (obj->get_children().empty()) flags |= ImGuiTreeNodeFlags_Leaf;
    bool open = ImGui::TreeNodeEx(name.c_str(), flags);
    if (ImGui::IsItemClicked()) {
        m_selected = obj;
        if (ImGui::IsMouseDoubleClicked(0)) {
            m_renaming = true;
        }
    }
    if (open) {
        for (auto it : obj->get_children()) {
            displayLobject(it);
        }
        ImGui::TreePop();
    }
}

// Components /////////////////////////////////////////////////////////////////

void DebugGui::componentWindow() {
    if (!m_selected) return;

    ImGui::Begin("Component Editor");

    ImGui::Text("%d", static_cast<int>(m_selected->m_entity));
    ImGui::SameLine();
    const char* current = "Add Component";
    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton;
    if (ImGui::BeginCombo("##Add Component", current, flags)) {
        if (ImGui::Selectable("Sprite")) {
            Sprite& sp = m_selected->add_component<Sprite>();
            m_scene->getRenderer()->loadSprite("../assets/smile.png", sp);
        }
        ImGui::EndCombo();
    }

    ImGui::Separator();
    familyComponent();
    ImGui::Separator();
    transformComponent();

    ImGui::End();
}

void DebugGui::familyComponent() {
    auto& comp = m_selected->get<Family>();
    ImGui::Text("Family Component");
    ImGui::Dummy(ImVec2(0, 10));

    ImGui::Text("Parent");
    ImGui::SameLine();
    ImGui::Text("%d", static_cast<int>(comp.parent));

    if (ImGui::Button("Set Root as Parent")) {
        m_scene->get_root()->add_child(m_selected);
    }

    ImGuiComboFlags flags = ImGuiComboFlags_PopupAlignLeft | ImGuiComboFlags_NoArrowButton;
    const char* current = "Add Child";
    if (ImGui::BeginCombo("##Add Child", current, flags)) {
        for (auto& i : m_scene->m_objects) {
            bool is_valid = true;
            auto climb = m_selected;
            while (climb != m_scene->get_root()) {
                if (i.second == climb) is_valid = false;
                climb = climb->get_parent();
            }
            for (auto ch : m_selected->get_children()) {
                if (i.second == ch) is_valid = false;
            }
            if (is_valid && ImGui::Selectable(i.second->get_name())) {
                m_selected->add_child(i.second);
            }
        }
        ImGui::EndCombo();
    }

    ImGui::Dummy(ImVec2(0, 10));

}

void DebugGui::transformComponent() {
    auto& comp = m_selected->get<Transform>();
    ImGui::Text("Transform Component");
    ImGui::Dummy(ImVec2(0, 10));

    float position[] = { comp.position.x, comp.position.y };

    ImGui::DragFloat2("position", position, 0.05f);
    ImGui::Dummy(ImVec2(0, 10));

    comp.position = { position[0], position[1] };
    /*
    rot = { rotation[0], rotation[1], rotation[2] };
    sca = { scale[0], scale[1], scale[2] };

    comp.recompose(pos, rot, sca);
     */

}

}