#include "SteamLobby.h"

SteamLobby::SteamLobby() {
}

SteamLobby::~SteamLobby() {
}

void SteamLobby::CreateLobby(int maxMembers) {
	SteamAPICall_t createLobbyCall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, maxMembers);
	m_SteamCallResultLobbyCreated.Set(createLobbyCall, this, &SteamLobby::OnLobbyCreated);
}

void SteamLobby::JoinLobby(CSteamID lobbyID) {
}

/*
void SteamLobby::LeaveLobby() {
	if (m_lobby) {
		SteamMatchmaking()->LeaveLobby(m_lobby);
		m_lobby = nullptr;
	}
}

CSteamID SteamLobby::GetLobbyOwner() const {
	if (m_lobby) {
		return SteamMatchmaking()->GetLobbyOwner(m_lobby);
	}
	return k_steamIDNil;
}

int SteamLobby::GetNumLobbyMembers() const {
	if (m_lobby) {
		return SteamMatchmaking()->GetNumLobbyMembers(m_lobby);
	}
	return 0;
}

CSteamID SteamLobby::GetLobbyMemberByIndex(int index) const {
	if (m_lobby) {
		return SteamMatchmaking()->GetLobbyMemberByIndex(m_lobby, index);
	}
	return k_steamIDNil;
}

bool SteamLobby::SendLobbyChatMessage(const char* message) {
	if (m_lobby) {
		return SteamMatchmaking()->SendLobbyChatMsg(m_lobby, message, strlen(message) + 1);
	}
	return false;
}
void SteamLobby::OnLobbyEntered(LobbyEnter_t* callback, bool failure) {
	if (failure) {
		// Handle lobby join failure.
		return;
	}

	m_lobby = callback->m_ulSteamIDLobby;
}
*/

void SteamLobby::OnMemberJoined(LobbyChatUpdate_t* callback) {
	// Handle member joined event.
}

void SteamLobby::OnMemberLeft(LobbyChatUpdate_t* callback) {
	// Handle member left event.
}

void SteamLobby::OnLobbyCreated(LobbyCreated_t* callback, bool failure) {

}

void SteamLobby::OnLobbyEntered(LobbyEnter_t* callback, bool failure) {

}
