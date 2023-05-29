#pragma once

#include "Renderer.h"
#include "NetworkSessionGGPO.h"
#include "GameplayManager.h"
#include "Input.h"
#include "Sprite.h"
#include "Scene.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace sng {
class NetworkSessionGGPO;

class TestScene {
public:
	TestScene(Input* input, Renderer* m_renderer);
	~TestScene();

	void Init(NetworkSessionGGPO* session);
	void update(long long dt);

private:
	// Gameplay has started
	bool m_init;

	NetworkSession* m_session;
	GameplayManager* m_gameplay;
	Input* m_input;
	Renderer* m_renderer;

    Scene* m_scene;

	int numPlayers;

	friend class Lobject;
};
}
