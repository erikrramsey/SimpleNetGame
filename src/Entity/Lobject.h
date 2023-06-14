#pragma once

#include <vector>
#include <string>

#include "Component.h"
#include "Scene.h"

namespace sng {

typedef unsigned long Entity;

// API to access an entity and it's components
class Scene;
class Lobject {
public: 
	Lobject(Entity id, Scene* scene);
	~Lobject();

	const char* get_name() { return m_name.c_str(); }
	void set_name(const char* name) { m_name = name; }
    void add_child(Lobject* child);

    Scene* get_scene() { return m_scene; }

	std::vector<Lobject*> get_children();
    Lobject* get_parent();

    template<class T>
    T& add_component();

	template<class T>
	T& add_component(T& comp);

	template<class T>
	T& get();

	template<class T>
	T* try_get();

	template<class T>
	void remove_component();

	std::string m_name;
	Entity m_entity;
private:
    ECRegistry& get_reg() const;
    Scene* m_scene;
};

template<class T>
T& Lobject::add_component() {
    if (try_get<T>() != nullptr) {
        std::cout << "Object already has component" << std::endl;
        return get<T>();
    }
    T comp(m_entity);
    get_reg().emplace(m_entity, comp);
    return get<T>();
}

template<class T>
T& Lobject::add_component(T& comp) {
	if (try_get<T>() != nullptr) {
		std::cout << "Object already has component" << std::endl;
		return get<T>();
	}
	get_reg().emplace(m_entity, comp);
	return get<T>();
}


template<class T>
T& Lobject::get() {
	return get_reg().get<T>(m_entity);
}

template<class T>
T* Lobject::try_get() {
	return get_reg().try_get<T>(m_entity);
}

template<class T>
void Lobject::remove_component() {
	get_reg().remove_component<T>(m_entity);
}

}

