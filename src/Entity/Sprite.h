#pragma once

#include "Component.h"

namespace  sng {
class Sprite : public Component {
public:
    Sprite(Entity self) : Component(self) {}
    int height = 0;
    int width = 0;
    unsigned int texture;
private:
};
}