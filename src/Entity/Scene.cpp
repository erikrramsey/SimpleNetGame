#include "Scene.h"
#include "Lobject.h"

namespace sng {
Scene::Scene() {
}

Scene::~Scene() {
}

void Scene::init() {
	m_renderer->setClearColor(glm::vec4(1.0f, 0.5f, 1.0f, 1.0f));

    m_root = new Lobject(m_registry.create(), this);
    m_root->set_name("root");
    m_root->add_component<Transform>();
    m_root->add_component<Family>();

    m_session = nullptr;
}

void Scene::clear() {
    for (auto i : m_objects) {
        clear_Lobject(i.second);
    }
    m_objects.clear();
}

void Scene::startOnlineSession(NetworkSessionGGPO *session) {
    if (session->getSessionState()->numPlayers < 2) {
        std::cout << "Session has only one player, not initializing ggpo" << std::endl;
    } else {
        m_manager = new GameplayManager();
        m_numPlayers = session->getSessionState()->numPlayers;
        for (int i = 0; i < m_numPlayers; i++) {
            auto obj = create_Lobject();
            obj->add_component<Player>();
            Sprite& sp = obj->add_component<Sprite>();
            getRenderer()->loadSprite("../assets/smile.png", sp);
        }


        m_session = session;
        m_session->setGameplayManager(m_manager);
        m_session->start_session();
    }
}

void Scene::update(long long dt) {
    m_renderer->begin();

    // Update gameplay logic
    if (m_session) {
        m_input->update();
        m_session->update(m_input->getInputs());
        auto& gs = m_manager->getGameState();

        auto& players = m_registry.get_pool<Player>();
        for (int i = 0; i < players.size(); i++) {
            get(players[i].get_ent())->get<Transform>().position = gs.players[i];
        }
    }

    // Render Sprites
	auto& children = m_registry.get_pool<Sprite>();
    for (auto i : children) {
        m_renderer->drawSprite(get(i.get_ent())->get<Transform>(), i);
    }

    m_renderer->end();
}

Lobject* Scene::create_Lobject() {
	auto* obj = new Lobject(m_registry.create(), this);
	obj->add_component<Transform>();
	obj->add_component<Family>();
	m_objects.emplace(obj->m_entity, obj);
    m_root->get<Family>().add_child(obj->get<Family>());
	return obj;
}

void Scene::delete_Lobject(Lobject* obj) {
    if (!obj) return;
	auto& fam = obj->get<Family>();
    auto par = m_registry.try_get<Family>(fam.parent);
    if (par) fam.remove_parent(*par);
	for (auto i : obj->get_children()) {
		delete_Lobject(i);
	}
	m_registry.delete_entity(obj->m_entity);
	m_objects.erase(obj->m_entity);
	delete obj;
    obj = nullptr;
}

void Scene::clear_Lobject(Lobject* obj) {
    auto& fam = obj->get<Family>();
    auto par = m_registry.try_get<Family>(fam.parent);
    if (par && fam.parent != 0) fam.remove_parent(*par);
    m_registry.delete_entity(obj->m_entity);
    delete obj;
    obj = nullptr;
}

Lobject* Scene::get(Entity ent) {
    if (ent == m_root->m_entity) return get_root();
	return m_objects.at(ent);
}

Lobject* Scene::get_root() {
    return m_root;
}

}