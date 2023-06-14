#ifndef SIMPLENETGAME_TRANSFORM_H
#define SIMPLENETGAME_TRANSFORM_H

#include "Component.h"
#include "glm/glm.hpp"

namespace sng {

class Transform : Component {
public:
    Transform(Entity self) : Component(self) {}

    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 rotation = glm::vec2(0.0f, 0.0f);
    glm::vec2 scale = glm::vec2(0.0f, 0.0f);
};

}


#endif //SIMPLENETGAME_TRANSFORM_H
