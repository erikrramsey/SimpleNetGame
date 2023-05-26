#pragma once

#include <vector>

#include "Entity.h"

namespace sng {

struct Component {
public:
	Component(Entity self) {
		m_self = self;
	}

	Entity get_ent() { return m_self; }

private:
	Entity m_self;
};

}
