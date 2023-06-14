#pragma once

#include "ECRegistry.h"

// Components
#include "Component.h"
#include "Player.h"
#include "Transform.h"
#include "Family.h"
#include "Body.h"
#include "StaticBody.h"

// Systems
#include "Renderer.h"
#include "Input.h"
#include "Physics.h"
#include "NetworkSessionGGPO.h"
#include "SessionState.h"

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

struct SharedState {
    glm::vec2  position[SessionState::MAX_PLAYERS] = {
            {0.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f},
    };

    glm::vec2  velocity[SessionState::MAX_PLAYERS] = {
            {0.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f},
    };
};

class Scene {
public:
	Scene();
	~Scene();

    // EC registry stuff
	void init();
	void update(long long dt);
    void clear();

    Lobject* get(Entity ent);
    Lobject* get_root();
    Lobject* create_Lobject();
    void delete_Lobject(Lobject* obj);
    void clear_Lobject(Lobject* obj);

    // Session stuff
    void startOnlineSession(NetworkSessionGGPO* session);
    void startOfflineSession();
    void startSyncTest(NetworkSessionGGPO* session);

    void advanceFrameCB(int inputs[SessionState::MAX_PLAYERS]);
    SharedState& getSharedState() { return m_sharedState; }
    void setSharedState(SharedState& sharedState) { m_sharedState = sharedState; }


    Renderer* getRenderer() { return m_renderer; }
    void setRenderer(Renderer* renderer) { m_renderer = renderer; }
    void setInput(Input* input) { m_input = input; }


	ECRegistry m_registry;
	std::unordered_map<Entity, Lobject*> m_objects;
private:
    SharedState m_sharedState;

    Lobject* m_root;
    Renderer* m_renderer;
    Input* m_input;
    NetworkSessionGGPO* m_session;

    bool m_gameInitialized;

    int m_numPlayers;

	friend class Lobject;


    std::unordered_map<Entity, int> players;

    void initBox2d();
};
}
