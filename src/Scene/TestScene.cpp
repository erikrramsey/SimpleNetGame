#include "TestScene.h"

namespace sng {
TestScene::TestScene(Input* input, Renderer* renderer) {
	m_init = false;
	m_input = input;
	m_renderer = renderer;
	m_gameplay = new GameplayManager();
}

TestScene::~TestScene() {
}

void TestScene::Init(NetworkSessionGGPO* session) {
    session->setGameplayManager(m_gameplay);
	m_session = session;
	numPlayers = m_session->getSessionState()->numPlayers;
	m_init = true;
	m_session->start_session();
}

void TestScene::update(long long dt) {
    /*
	if (!m_init)
		return;

	m_input->update();
	m_session->update(m_input->getInputs());
	auto& gs = m_gameplay->getGameState();

     */
}

}