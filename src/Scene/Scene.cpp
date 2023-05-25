#include "Scene.h"

namespace sng {
Scene::Scene(Input* input, Renderer* renderer) {
	m_init = false;
	m_input = input;
	m_renderer = renderer;
	m_gameplay = new GameplayManager();
}

Scene::~Scene() {
}

void Scene::Init(NetworkSessionGGPO* session) {
	session->SetGameplayManager(m_gameplay);
	m_session = session;
	numPlayers = m_session->getSessionState()->numPlayers;
	m_init = true;
    penguSprite = SpriteLoader::load("../assets/pengu.jpg");
	m_session->start_session();
}

void Scene::update(long long dt) {
	if (!m_init)
		return;

	m_input->update();
	m_session->update(m_input->getInputs());
	auto& gs = m_gameplay->getGameState();

	m_renderer->begin();

	for (int i = 0; i < numPlayers; i++) {
		m_renderer->drawQuad(gs.players[i].tr.position);
	}

	m_renderer->end();
}

}