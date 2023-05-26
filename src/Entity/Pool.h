#pragma once
#include "Entity.h"

#include <typeinfo>
#include <vector>

namespace sng {

class IPool {
public:
	virtual void remove(Entity ent) = 0;
};

// Implementation of a sparse set with an underlying component vector
template <class T>
class Pool : public IPool {
public:

	Pool();
	~Pool();

	// Insertion deletion search/retrieve
	void emplace(T comp, Entity ent);
	void remove(Entity ent);
	T& get(Entity ent);
	T* try_get(Entity ent);

	void clear();
	void dummy() {}

	std::vector<T>& get_components();

private:
	Entity* m_sparse;
	Entity* m_dense;
	std::vector<T> m_components;

	unsigned int m_size;
};

template<class T>
Pool<T>::Pool() {
	m_sparse = new Entity[MAX_ENTS + 1];
	m_dense = new Entity[MAX_ENTS + 1];

	m_size = 0;
}

template<class T>
Pool<T>::~Pool() {
	delete m_sparse;
	delete m_dense;

	m_sparse = m_dense = nullptr;
	m_size = 0;
}


template<class T>
void Pool<T>::emplace(T comp, Entity ent) {
	m_components.push_back(comp);
	m_sparse[ent] = m_size;
	m_dense[m_size] = ent;
	m_size++;
}

template<class T>
T& Pool<T>::get(Entity ent) {
	return m_components[m_sparse[ent]];
}

template<class T>
T* Pool<T>::try_get(Entity ent) {
	if (m_sparse[ent] < m_size && m_dense[m_sparse[ent]] == ent)
		return &m_components[m_sparse[ent]];
	return nullptr;
}

template<class T>
void Pool<T>::remove(Entity ent) {
	if (m_sparse[ent] >= m_size || m_dense[m_sparse[ent]] != ent)
		return;

	m_components[m_sparse[ent]] = m_components.back();
	m_components.pop_back();

	Entity temp = m_dense[m_size - 1];
	m_dense[m_sparse[ent]] = temp;
	m_sparse[temp] = m_sparse[ent];

	m_size--;
}

template<class T>
void Pool<T>::clear() {
	m_size = 0;
	m_components.clear();
}

template<class T>
std::vector<T>& Pool<T>::get_components() {
	return m_components;
}

}
