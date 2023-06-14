#ifndef SIMPLENETGAME_STATICBODY_H
#define SIMPLENETGAME_STATICBODY_H

#include <glm/glm.hpp>
#include "Component.h"

namespace sng {

class StaticBody : public Component  {
public:
    StaticBody(Entity self) : Component(self) {}

    glm::vec2 dir {0.0f, 0.0f};
    glm::vec2 aabb {0.0f, 0.0f};
private:
};

}
#endif //SIMPLENETGAME_STATICBODY_H
