#include "Lobject.h"

namespace sng {

Lobject::Lobject(Entity id, Scene* scene) {
	m_entity = id;
	m_scene = scene;
	m_name = "Object #" + std::to_string(id);
}

Lobject::~Lobject() {}

std::vector<Lobject*> Lobject::get_children() {
	std::vector<Lobject*> children;
	for (auto i : get<Family>().children) {
		children.push_back(m_scene->get(i));
	}
	return children;
}

Lobject* Lobject::get_parent() {
    return (m_scene->get(get<Family>().parent));
}

void Lobject::add_child(Lobject* child) {
    auto fam = child->get<Family>();
    fam.remove_parent(m_scene->get(fam.parent)->get<Family>());
    get<Family>().add_child(child->get<Family>());
}

ECRegistry& Lobject::get_reg() const { return m_scene->m_registry; }

}