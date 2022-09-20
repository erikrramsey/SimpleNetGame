#pragma once

#include "ggponet.h"

namespace sng {
struct SessionState {
	const static int MAX_PLAYERS = 4;

	GGPOSession* ggpo;
	GGPOPlayer players[MAX_PLAYERS];
	GGPOPlayerHandle player_handles[MAX_PLAYERS];
	GGPOSessionCallbacks cb;

	unsigned int numPlayers;
	int localPort;
	int localPlayer;

};
}

