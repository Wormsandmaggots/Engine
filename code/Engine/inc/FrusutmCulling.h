#ifndef ENGINE_FRUSUTMCULLING_H
#define ENGINE_FRUSUTMCULLING_H

#include "Frustum.h"
#include "Scene/Transform2.h"
#include "AABBComponent.h"
#include "AABB.hpp"

class FrustumCulling {
public:
    static bool isOnFrustum(const Frustum& camFrustum, const Transform2& transform, const AABBComponent& aabbComponent) {
        // Get global scale thanks to our transform
        const glm::vec3 globalCenter{ transform.getModelMatrix() * glm::vec4(aabbComponent.center, 1.f) };

        // Scaled orientation
        const glm::vec3 right = transform.getRight() * aabbComponent.extents.x;
        const glm::vec3 up = transform.getUp() * aabbComponent.extents.y;
        const glm::vec3 forward = transform.getForward() * aabbComponent.extents.z;

        const float newIi = std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 1.f, 0.f, 0.f }, forward));

        const float newIj = std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 1.f, 0.f }, forward));

        const float newIk = std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, right)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, up)) +
                            std::abs(glm::dot(glm::vec3{ 0.f, 0.f, 1.f }, forward));

        // We not need to divide scale because it's based on the half extension of the AABB
        const AABB globalAABB(globalCenter, newIi, newIj, newIk);

        return (globalAABB.isOnOrForwardPlane(camFrustum.leftFace) &&
                globalAABB.isOnOrForwardPlane(camFrustum.rightFace) &&
                globalAABB.isOnOrForwardPlane(camFrustum.topFace) &&
                globalAABB.isOnOrForwardPlane(camFrustum.bottomFace) &&
                globalAABB.isOnOrForwardPlane(camFrustum.nearFace) &&
                globalAABB.isOnOrForwardPlane(camFrustum.farFace));
    }
};
#endif