#pragma once

#include "ECRegistry.h"
#include "Component.h"
#include "Transform.h"
#include "Family.h"

#include "Renderer.h"

#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iterator>

namespace sng {

class SceneSerializer;
class Lobject;
class Importer;
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
    void setRenderer(Renderer* renderer) { m_renderer = renderer; }
	
	ECRegistry m_registry;
	std::unordered_map<Entity, Lobject*> m_objects;
private:
    Lobject* m_root;
    Renderer* m_renderer;

	friend class Lobject;
};
}
