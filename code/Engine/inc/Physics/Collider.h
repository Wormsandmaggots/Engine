//
// Created by Radek on 26.03.2024.
//

#ifndef ENGINE_COLLIDER_H
#define ENGINE_COLLIDER_H

#include "AABB.hpp"

using namespace CPM_GLM_AABB_NS;

class Collider {
public:
    Collider(glm::vec3&, float);
    virtual ~Collider() = default;

    [[nodiscard]] bool CheckCollision(const Collider& other) const {
        return aabb.overlaps(other.aabb);
    }

private:
    AABB aabb;
};


#endif //ENGINE_COLLIDER_H
