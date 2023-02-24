#pragma once

#include <steam/steam_api.h>
#include "steam/isteamnetworkingutils.h"
#include "steam/steam_api_common.h"

class SteamLobby {
public:
	SteamLobby();
	~SteamLobby();

	void CreateLobby(int maxMembers);
	void JoinLobby(CSteamID lobbyID);
	//void LeaveLobby();

	CSteamID GetLobbyOwner() const;
	int GetNumLobbyMembers() const;
	CSteamID GetLobbyMemberByIndex(int index) const;
	bool SendLobbyChatMessage(const char* message);
	
private:
	CSteamID m_lobby;

	CCallResult<SteamLobby, LobbyCreated_t> m_SteamCallResultLobbyCreated;

	void OnLobbyCreated(LobbyCreated_t* callback, bool failure);
	void OnLobbyEntered(LobbyEnter_t* callback, bool failure);
	void OnMemberJoined(LobbyChatUpdate_t * callback);
	void OnMemberLeft(LobbyChatUpdate_t * callback);
};
