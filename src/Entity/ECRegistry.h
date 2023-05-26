#pragma once
#include "Pool.h"
#include "Entity.h"

#include <vector>
#include <iostream>
#include <unordered_set>

namespace sng {

class ECRegistry {
public:
	ECRegistry();
	~ECRegistry();

	// return a unique id belonging to an entity
	Entity create();
	void delete_entity(Entity ent);

	template<class T>
	void remove_component(Entity ent);

	// emplace a new component of type t into the vector? of them,
	// if one does not already exist create the vecotr using component_register()
	template<class T>
	void emplace(Entity ent, T comp);

	// get ref to entity if it exists
	template<class T>
	T& get(Entity ent);

	template<class T>
	T* try_get(Entity ent);

	template<class T>
	std::vector<T>& get_pool();

	template<class T>
	Pool<T>* pool_exists();
private:
	unsigned long m_next_id = 0;
	std::unordered_set<unsigned long> m_ents;

	std::vector<IPool*> m_pools;
	// create a vector of type t to store all components of a type
	template<class T>
	void component_register(T& type, Entity ent);

	template<class T>
	void pool_register();
};

template<class T>
void ECRegistry::emplace(Entity ent, T comp) {
	if (!m_pools.empty()) {
		Pool<T>* pool = pool_exists<T>();
		if (pool != nullptr) {
			return pool->emplace(comp, ent);
		}
	}
	component_register(comp, ent);
}

template<class T>
T& ECRegistry::get(Entity ent) {
	return pool_exists<T>()->get(ent);
}

template<class T>
T* ECRegistry::try_get(Entity ent) {
	auto pool = pool_exists<T>();
	if (m_pools.empty() || !pool) return nullptr;
	return pool->try_get(ent);
}

template<class T>
void ECRegistry::remove_component(Entity ent) {
	Pool<T>* pool = pool_exists<T>();
	pool->remove(ent);
}

template<class T>
std::vector<T>& ECRegistry::get_pool() {
	auto p = pool_exists<T>();
	if (!p) {
		pool_register<T>();
	}
	return pool_exists<T>()->get_components();
}

template<class T>
void ECRegistry::component_register(T& comp, Entity ent) {
	Pool<T>* new_pool = new Pool<T>;
	m_pools.push_back(new_pool);
	new_pool->emplace(comp, ent);
}

template<class T>
void ECRegistry::pool_register() {
	Pool<T>* new_pool = new Pool<T>;
	m_pools.push_back(new_pool);
}

template<class T>
Pool<T>* ECRegistry::pool_exists() {
	for (auto pool : m_pools) {
		Pool<T>* test_pool = dynamic_cast<Pool<T>*>(pool);
		if (test_pool != nullptr) {
			return test_pool;
		}
	}
	return nullptr;
}

}