#pragma once

#include "Sprite.h"
#include <string>

namespace sng {

class SpriteLoader {
public:
    static Sprite load(const std::string& filename);
};

}