#include "GameplayManager.h"

namespace sng {
GameState& GameplayManager::getGameState() { return m_gameState; }

void GameplayManager::advanceFrame(int inputs[SessionState::MAX_PLAYERS]) {
	bool newInput = false;
	for (int i = 0; i < 4; i++) {
		m_gameState.positions[i] += inputs[i];
		if (inputs[i])
			newInput = true;
	}

	if (newInput) {
		for (int i = 0; i < 4; i++)
			std::cout << m_gameState.positions[i] << ' ';
		std::cout << std::endl;
	}

}

}