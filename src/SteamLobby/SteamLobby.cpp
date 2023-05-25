#include "SteamLobby.h"

namespace sng {

SteamLobby::SteamLobby() {
}

SteamLobby::~SteamLobby() {
}

void SteamLobby::CreateLobby(int maxMembers) {
	SteamAPICall_t createLobbyCall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, maxMembers);
	m_SteamCallResultLobbyCreated.Set(createLobbyCall, this, &SteamLobby::OnLobbyCreated);
}

void SteamLobby::JoinLobby(CSteamID lobbyID) {
	SteamAPICall_t createLobbyCall = SteamMatchmaking()->JoinLobby(lobbyID);
	m_SteamCallResultLobbyEntered.Set(createLobbyCall, this, &SteamLobby::OnLobbyEntered);
}

int SteamLobby::GetNumLobbyMembers() const {
	if (m_lobby.IsValid())
		return SteamMatchmaking()->GetNumLobbyMembers(m_lobby);

	return 0;
}

CSteamID SteamLobby::GetLobbyMemberByIndex(int index) const {
	if (m_lobby.IsValid()) {
		return SteamMatchmaking()->GetLobbyMemberByIndex(m_lobby, index);
	}
	return k_steamIDNil;
}

CSteamID SteamLobby::GetId() const {
	return m_lobby;
}

const char* SteamLobby::GetData(const char* key) const {
	return SteamMatchmaking()->GetLobbyData(m_lobby, key);
}

void SteamLobby::SetData(const char* key, const char* data) {
	SteamMatchmaking()->SetLobbyData(m_lobby, key, data);
}

const char* SteamLobby::GetUserData(CSteamID user, const char* key) {
	return SteamMatchmaking()->GetLobbyMemberData(m_lobby, user, key);
}

void SteamLobby::SetUserData(const char* key, const char* data) {
	SteamMatchmaking()->SetLobbyMemberData(m_lobby, key, data);
}

void SteamLobby::CreateNetworkSession() {
	NetworkSessionGGPO* session = new NetworkSessionGGPO();

	for (int i = 0; i < GetNumLobbyMembers(); i++) {
		CSteamID player = GetLobbyMemberByIndex(i);
		CSteamID self = SteamUser()->GetSteamID();
		if (player == self) {
			session->add_player("local", std::atoi(GetUserData(player, "Port")));
			continue;
		}
		if (!strcmp(GetUserData(player, "IP"), GetUserData(self, "IP"))) {
			session->add_player(GetUserData(player, "LocalIP"), std::atoi(GetUserData(player, "Port")));
			continue;
		}
		
		session->add_player(GetUserData(player, "IP"), std::atoi(GetUserData(player, "Port")));
	}

	Application::GetCurrentScene().Init(session);
}

void SteamLobby::OnMemberJoined(LobbyChatUpdate_t* callback) {
}

void SteamLobby::OnMemberLeft(LobbyChatUpdate_t* callback) {
}

void SteamLobby::OnLobbyCreated(LobbyCreated_t* callback, bool failure) {
	m_lobby = callback->m_ulSteamIDLobby;
}

// Called when joining other lobbies ?
void SteamLobby::OnLobbyEntered(LobbyEnter_t* callback, bool failure) {
	m_lobby = callback->m_ulSteamIDLobby;
	SetUserData("IP", AddressFinder::GetPublicIPAddress().c_str());
	SetUserData("LocalIP", AddressFinder::GetLocalIPAddress().c_str());
	SetUserData("Port", "7001");
}

// Called after creating a lobby ?
void SteamLobby::OnLobbyEntered(LobbyEnter_t* callback) {
	m_lobby = callback->m_ulSteamIDLobby;
	SetUserData("IP", AddressFinder::GetPublicIPAddress().c_str());
	SetUserData("LocalIP", AddressFinder::GetLocalIPAddress().c_str());
	SetUserData("Port", "7001");
}

void SteamLobby::OnLobbyJoinRequested(GameLobbyJoinRequested_t* callback) {
	JoinLobby(callback->m_steamIDLobby);
}

void SteamLobby::OnLobbyDataUpdate(LobbyDataUpdate_t* callback) {
	if (!strcmp(GetData("GameStart"), "true"))
		CreateNetworkSession();
}


}
