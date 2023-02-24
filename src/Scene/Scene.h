#pragma once

#include "Renderer.h"
#include "NetworkSessionGGPO.h"
#include "GameplayManager.h"
#include "Input.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace sng {

class Scene {
public:
	Scene(int argc, char** argv, Input* input, Renderer* m_renderer);
	~Scene();

	void Init();
	void update(long long dt);

private:
	NetworkSession* m_session;
	GameplayManager* m_gameplay;
	Input* m_input;
	Renderer* m_renderer;

	int numPlayers;

	friend class Lobject;
};
}
