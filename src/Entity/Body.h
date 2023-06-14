#ifndef SIMPLENETGAME_BODY_H
#define SIMPLENETGAME_BODY_H

#include <glm/glm.hpp>
#include "Component.h"

namespace sng {

class Body : public Component  {
public:
    Body(Entity self) : Component(self) {}

    glm::vec2 velocity = {0.0f, 0.0f};
    glm::vec2 aabb {0.0f, 0.0f};
private:
};

}
#endif //SIMPLENETGAME_BODY_H
