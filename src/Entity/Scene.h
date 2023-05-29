#pragma once

#include "ECRegistry.h"
#include "Component.h"
#include "Player.h"
#include "Transform.h"
#include "Family.h"

#include "Renderer.h"
#include "Input.h"
#include "NetworkSessionGGPO.h"
#include "GameplayManager.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace sng {

class SceneSerializer;
class Lobject;
class Importer;
class NetworkSessionGGPO;
class Renderer;
class GameplayManager;

class Scene {
public:
	Scene();
	~Scene();

	void init();
	void update(long long dt);
    void clear();

	Lobject* get(Entity ent);
    Lobject* get_root();
	Lobject* create_Lobject();
	void delete_Lobject(Lobject* obj);
    void clear_Lobject(Lobject* obj);

    Renderer* getRenderer() { return m_renderer; }
    void setRenderer(Renderer* renderer) { m_renderer = renderer; }
    void setInput(Input* input) { m_input = input; }

    void startOnlineSession(NetworkSessionGGPO* session);

	ECRegistry m_registry;
	std::unordered_map<Entity, Lobject*> m_objects;
private:
    Lobject* m_root;
    Renderer* m_renderer;
    Input* m_input;
    NetworkSessionGGPO* m_session;
    GameplayManager* m_manager;

    int m_numPlayers;

	friend class Lobject;
};
}
