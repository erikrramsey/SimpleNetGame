#ifndef SIMPLENETGAME_PLAYER_H
#define SIMPLENETGAME_PLAYER_H

#include "Component.h"

namespace sng {

enum PlayerState {
    INVALID = 0,
    Grounded = 1,
    Falling = 2,
};

class Player : public Component {
public:
    Player(Entity self) : Component(self) {}
    int playerHandle = 0;
    PlayerState state = INVALID;
};

}
#endif //SIMPLENETGAME_PLAYER_H
