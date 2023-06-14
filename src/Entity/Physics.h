#ifndef SIMPLENETGAME_PHYSICS_H
#define SIMPLENETGAME_PHYSICS_H

#include "Body.h"
#include "StaticBody.h"
#include "Transform.h"

#include "ECRegistry.h"

namespace sng {

class Physics {
public:
    static void update(ECRegistry& registry, float timeStep);
};

}


#endif //SIMPLENETGAME_PHYSICS_H
