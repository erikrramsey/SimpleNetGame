#pragma once

#include <iostream>

#include "SessionState.h"

#include "Scene.h"


namespace sng {

class Scene;

struct GameState {
    const static int MAX_PLAYERS = 4;
    glm::vec2 players[MAX_PLAYERS];
};

class GameplayManager {
public:
	GameState& getGameState();
	void advanceFrame(char inputs[SessionState::MAX_PLAYERS]);

    void setScene(Scene* scene) { m_scene = scene; }
private:
	GameState m_gameState;
    Scene* m_scene;
};

}