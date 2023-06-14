#pragma once

#include <stddef.h>
#include "steam/steam_api.h"
#include "steam/isteamutils.h"
#include "steam/steamclientpublic.h"

#include "Application.h"
#include "AddressFinder.h"
#include "NetworkSessionGGPO.h"

namespace sng {

class SteamLobby {
public:
	SteamLobby();
	~SteamLobby();


	void CreateLobby(int maxMembers);
	void JoinLobby(CSteamID lobbyID);
	//void LeaveLobby();

	CSteamID GetId() const;
	CSteamID GetLobbyOwner() const;

	const char* GetData(const char* key) const;
	void SetData(const char* key, const char* data);

	const char* GetUserData(CSteamID user, const char* key);
	void SetUserData(const char* key, const char* data);

	int GetNumLobbyMembers() const;
	CSteamID GetLobbyMemberByIndex(int index) const;
	bool IsActive() { return m_lobby.IsValid(); }

	void CreateNetworkSession();
private:
	CSteamID m_lobby;

	CCallResult<SteamLobby, LobbyCreated_t> m_SteamCallResultLobbyCreated;
	CCallResult<SteamLobby, LobbyEnter_t> m_SteamCallResultLobbyEntered;

	STEAM_CALLBACK(SteamLobby, OnLobbyEntered, LobbyEnter_t);
	STEAM_CALLBACK(SteamLobby, OnLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(SteamLobby, OnLobbyDataUpdate, LobbyDataUpdate_t);

	void OnLobbyCreated(LobbyCreated_t* callback, bool failure);
	void OnLobbyEntered(LobbyEnter_t* callback, bool failure);
	void OnMemberJoined(LobbyChatUpdate_t* callback);
	void OnMemberLeft(LobbyChatUpdate_t* callback);
};

}
