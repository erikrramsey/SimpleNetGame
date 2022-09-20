#pragma once

#include <iostream>

#include "GameState.h"
#include "SessionState.h"

namespace sng {

class GameplayManager {
public:
	GameState& getGameState();
	void advanceFrame(int inputs[SessionState::MAX_PLAYERS]);
private:
	GameState m_gameState;
};

}