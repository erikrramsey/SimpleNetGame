#pragma once
#include "Component.h"
#include "Entity.h"

#include <vector>
#include <algorithm>

namespace sng {

struct Family : public Component {
public:
	Family(Entity self) : Component(self) {
		parent = 0;
	}

	void set_parent(Family& other) {
		parent = other.get_ent();
	}

    void remove_parent(Family& parent_) {
        parent_.remove_child(*this);
    }

    void add_child(Family& other) {
        other.set_parent(*this);
        children.push_back(other.get_ent());
    }

	void remove_child(Family& other) {
		other.parent = 0;
		children.erase(std::remove(children.begin(), children.end(), other.get_ent()));
	}


	Entity parent;
	std::vector<Entity> children;
};

}