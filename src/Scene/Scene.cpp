#include "Scene.h"

namespace sng {
Scene::Scene(int argc, char** argv, Input* input, Renderer* renderer) {
	m_input = input;
	m_renderer = renderer;
	m_gameplay = new GameplayManager();
	m_session = new NetworkSessionGGPO(m_gameplay);

	for (auto i = 1; i < argc; i+= 2) {
		m_session->add_player(argv[i + 1], atoi(argv[i]));
	}

	std::cout << "Starting session" << std::endl;
	m_session->start_session();


	numPlayers = m_session->getSessionState()->numPlayers;

}

Scene::~Scene() {
}

void Scene::Init() {
}

void Scene::update(long long dt) {
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