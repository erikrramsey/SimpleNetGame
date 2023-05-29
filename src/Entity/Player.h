#ifndef SIMPLENETGAME_PLAYER_H
#define SIMPLENETGAME_PLAYER_H

#include "Component.h"

namespace sng {

class Player : public Component {
public:
    Player(Entity self) : Component(self) {}
    int playerHandle = 0;
};

}
#endif //SIMPLENETGAME_PLAYER_H
