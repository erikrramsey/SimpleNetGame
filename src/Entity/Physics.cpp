
#include "Physics.h"

const glm::vec2 gravity = {0.0f, -9.8f};

bool overlap(const glm::vec2& a, const glm::vec2& ab, const glm::vec2& b, const glm::vec2& bb) {
    //static int calls = 0;
    //std::cout << calls++ << std::endl;
    return a.x < b.x + bb.x
    && a.x + ab.x > b.x
    && a.y < b.y + bb.y
    && a.y + ab.y > b.y;
}

glm::vec2 center(const glm::vec2& tl, const glm::vec2& bb) {
    return tl + glm::vec2(bb.x / 2.0f, bb.y / 2.0f);
}

namespace sng {
void Physics::update(sng::ECRegistry& registry, float timeStep) {
    auto& bodies = registry.get_pool<Body>();
    auto& ground = registry.get_pool<StaticBody>();

    for (auto& body : bodies) {
        auto& t = registry.get<Transform>(body.ent);
        t.position += (body.velocity * timeStep);
        body.velocity += gravity * timeStep;
        body.velocity.x *= 0.9f;

        for (auto& cbody : ground) {
            if (cbody.ent == body.ent) continue;
            auto& ct = registry.get<Transform>(cbody.ent);
            while (overlap(t.position, body.aabb, ct.position, cbody.aabb)) {
                body.velocity *= glm::vec2(cbody.dir.y * cbody.dir.y, cbody.dir.x * cbody.dir.x);
                t.position += cbody.dir * 0.01f;
            }
        }
    }
}

}
