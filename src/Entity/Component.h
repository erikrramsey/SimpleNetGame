#pragma once

#include <vector>

#include "Entity.h"

namespace sng {

struct Component {
public:
	Component(Entity self) {
		ent = self;
	}

	Entity get_ent() { return ent; }
    Entity ent;
private:
};

}
