//
// Created by Radek on 26.03.2024.
//

#include "Physics/Collider.h"

//later it should be changed to check the collision of
//option 1 - transform should have center as a member
//option 2 - it should have a transform matrix as a member
Collider::Collider(glm::vec3 &center, float radius) {
    aabb = AABB(center, radius);
}
