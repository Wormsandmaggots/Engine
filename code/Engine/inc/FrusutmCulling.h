#ifndef ENGINE_FRUSUTMCULLING_H
#define ENGINE_FRUSUTMCULLING_H

#include "Frustum.h"
#include "Scene/Transform2.h"
#include "AABBComponent.h"
#include "AABB.hpp"

class FrustumCulling {
public:
    bool isOnFrustum(const Frustum& camFrustum, const AABB& aabb) const
    {
        // Get center and extents from the AABB
        const glm::vec3 globalCenter = aabb.center;
        const glm::vec3 extents = aabb.extents;

        // Scaled orientation
        const glm::vec3 right = globalCenter * extents.x;
        const glm::vec3 up = globalCenter * extents.y;
        const glm::vec3 forward = globalCenter * extents.z;

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