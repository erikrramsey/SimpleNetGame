#include "GameplayManager.h"

namespace sng {
GameState& GameplayManager::getGameState() { return m_gameState; }
void GameplayManager::advanceFrame(int inputs[SessionState::MAX_PLAYERS]) {
	m_gameState.position1 += inputs[0];
	m_gameState.position2 += inputs[1];

	if (inputs[0] || inputs[1]) {
		std::cout << m_gameState.position1 << ' ' << m_gameState.position2 << std::endl;
	}
}

}