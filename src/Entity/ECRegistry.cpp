#include "ECRegistry.h"

namespace sng {

	ECRegistry::ECRegistry() {

	}

	ECRegistry::~ECRegistry() {

	}

	Entity ECRegistry::create() {
		m_ents.emplace(m_next_id++);
		return m_next_id - 1;
	}


    void ECRegistry::delete_entity(Entity ent) {
		for (auto& pool : m_pools) {
			pool->remove(ent);
		}
    }
}