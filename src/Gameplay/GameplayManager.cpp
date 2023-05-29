#include "GameplayManager.h"

namespace sng {
GameState& GameplayManager::getGameState() { return m_gameState; }

void GameplayManager::advanceFrame(char inputs[SessionState::MAX_PLAYERS]) {
	for (int i = 0; i < 4; i++) {
		auto& pos = m_gameState.players[i];
		switch (inputs[i]) {
			case 'a':
				pos.x -= 1;
				break;
			case 'w':
				pos.y += 1;
				break;
			case 'd':
				pos.x += 1;
				break;
			case 's':
				pos.y -= 1;
				break;
			default: break;
		}
	}
}

}