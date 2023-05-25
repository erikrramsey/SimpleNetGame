#pragma once

#include "Renderer.h"
#include "NetworkSessionGGPO.h"
#include "GameplayManager.h"
#include "Input.h"
#include "SpriteLoader.h"
#include "Sprite.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace sng {
class NetworkSessionGGPO;

class Scene {
public:
	Scene(Input* input, Renderer* m_renderer);
	~Scene();

	void Init(NetworkSessionGGPO* session);
	void update(long long dt);

private:
	// Gameplay has started
	bool m_init;

	NetworkSession* m_session;
	GameplayManager* m_gameplay;
	Input* m_input;
	Renderer* m_renderer;

    Sprite penguSprite;

	int numPlayers;

	friend class Lobject;
};
}
