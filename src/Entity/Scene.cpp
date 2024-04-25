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
    m_gameInitialized = false;

}

void Scene::clear() {
    for (auto i : m_objects) {
        clear_Lobject(i.second);
    }
    m_objects.clear();
}


void Scene::startOnlineSession(NetworkSessionGGPO *session) {
    if (m_gameInitialized) return;

    if (session->getSessionState()->numPlayers < 2) {
        std::cout << "Lobby has only one player, can't begin online session" << std::endl;
    } else {
        std::cout << "Lobby has more than one player, loading ggpo scene" << std::endl;
        m_numPlayers = session->getSessionState()->numPlayers;
        for (int i = 0; i < m_numPlayers; i++) {
            auto obj = create_Lobject();

            obj->add_component<Player>();
            players.insert({i, obj->m_entity});

            Sprite& sp = obj->add_component<Sprite>();
            obj->get<Transform>().position = { 3.0f * i + 1.0f, 5.0f };
            obj->get<Transform>().scale = { 1.0f, 1.0f };

            auto& body = obj->add_component<Body>();
            body.aabb = {1.0f, 1.0f};

            m_sharedState.position[i] = obj->get<Transform>().position;
            getRenderer()->loadSprite("../assets/smile.png", sp);
        }

        {
            auto obj = create_Lobject();
            obj->get<Transform>().position = {0.0f, 0.2f};
            obj->get<Transform>().scale = {16.0f, 0.2f};
            auto &body = obj->add_component<StaticBody>();
            body.aabb = {16.0f, 0.2f};
            body.dir = {0.0f, 1.0f};
            obj->add_component<Sprite>();
        }

        {
            auto obj = create_Lobject();
            obj->get<Transform>().position = {0.0f, 8.0f};
            obj->get<Transform>().scale = {16.0f, 0.2f};
            auto &body = obj->add_component<StaticBody>();
            body.aabb = {16.0f, 0.2f};
            body.dir = {0.0f, -1.0f};
            obj->add_component<Sprite>();
        }

        {
            auto obj = create_Lobject();
            obj->get<Transform>().position = {4.0f, 4.0f};
            obj->get<Transform>().scale = {8.0f, 0.2f};
            auto &body = obj->add_component<StaticBody>();
            body.aabb = {8.0f, 0.2f};
            body.dir = {0.0f, -1.0f};
            obj->add_component<Sprite>();
        }

        {
            auto obj = create_Lobject();
            obj->get<Transform>().position = {0.0f, 0.2f};
            obj->get<Transform>().scale = {0.2f, 9.0f};
            auto &body = obj->add_component<StaticBody>();
            body.aabb = {0.2f, 5.0f};
            body.dir = {1.0f, 0.0f};
            obj->add_component<Sprite>();
        }

        {
            auto obj = create_Lobject();
            obj->get<Transform>().position = {15.8f, 0.2f};
            obj->get<Transform>().scale = {0.2f, 9.0f};
            auto &body = obj->add_component<StaticBody>();
            body.aabb = {0.2f, 9.0f};
            body.dir = {-1.0f, 0.0f};
            obj->add_component<Sprite>();
        }

        m_session = session;
        m_session->setScene(this);
        m_session->start_session();

        m_gameInitialized = true;

        initBox2d();
    }
}

void Scene::startOfflineSession() {
    if (m_gameInitialized) return;

    auto player = create_Lobject();
    player->add_component<Player>();
    getRenderer()->loadSprite("../assets/smile.png", player->add_component<Sprite>());

    players.insert({0, player->m_entity});

    initBox2d();

    m_gameInitialized = true;
}

void Scene::startSyncTest(NetworkSessionGGPO* session) {
    auto player = create_Lobject();
    player->add_component<Player>();
    getRenderer()->loadSprite("../assets/smile.png", player->add_component<Sprite>());

    players.insert({0, player->m_entity});

    initBox2d();

    session->add_player("local", 7777);
    session->startSyncTest();
    m_gameInitialized = true;
    m_session = session;
    m_session->setScene(this);
}


void Scene::update(long long dt) {
    m_renderer->begin();

    if (m_gameInitialized) {
        // Update session which will call back to update game state
        if (m_session) {
            m_input->update();
            m_session->update(m_input->getInputs());
        } else {
            m_input->update();
            int inputs[SessionState::MAX_PLAYERS] = {m_input->getInputs()};
            advanceFrameCB(inputs);
        }
    }

    // Render Sprites
	auto& children = m_registry.get_pool<Sprite>();
    for (auto i : children) {
        m_renderer->drawSprite(get(i.get_ent())->get<Transform>(), i);
    }

    m_renderer->end();
}

void Scene::advanceFrameCB(int inputs[SessionState::MAX_PLAYERS]) {
    for (auto pair : players) {
        auto &body = get(pair.second)->get<Body>();
        auto &tr = get(pair.second)->get<Transform>();
        body.velocity = m_sharedState.velocity[pair.first];
        tr.position = m_sharedState.position[pair.first];

        switch (inputs[pair.first]) {
            case 'a':
                body.velocity += glm::vec2(-1.0f, 0.0f);
                break;
            case 'd':
                body.velocity += glm::vec2(1.0f, 0.0f);
                break;
            case 'w':
                body.velocity += glm::vec2(0.0f, 1.0f);
                break;
            case 's':
                body.velocity += glm::vec2(0.0f, -1.0f);
                break;
            default:
                break;
        }
    }

    Physics::update(this->m_registry, 1.0f/60.0f);

    for (auto pair : players) {
        auto &body = get(pair.second)->get<Body>();
        auto &tr = get(pair.second)->get<Transform>();
        m_sharedState.position[pair.first] = tr.position;
        m_sharedState.velocity[pair.first] = body.velocity;
    }

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

void Scene::initBox2d() {
}

}