#ifndef ENGINE_AABBCOMPONENT_H
#define ENGINE_AABBCOMPONENT_H

#include "ECS/Component.h"
#include "AABB.hpp"
#include "LineRenderer.h"
#include "Frustum.h"
#include "Scene/Transform2.h"

class AABBComponent : public Component
{
public:
    CPM_GLM_AABB_NS::AABB aabb;
    Transform2 transform;
    Frustum::AABB aabb2;

    AABBComponent() = default;

    Transform2* getTransform() {
        return &transform;
    }

    void awake() override {
        // Implementacja metody awake
    }

    void start() override {
        // Implementacja metody start
    }

    void update() override {
        // Implementacja metody update
    }

    void onDestroy() override {
        // Implementacja metody onDestroy
    }

    void setAABB(const CPM_GLM_AABB_NS::AABB& newAABB) {
        aabb = newAABB;
    }

    CPM_GLM_AABB_NS::AABB getAABB() const {
        return aabb;
    }

    bool isOnFrustum(const Frustum& camFrustum, const Transform2& transform) const {
        return aabb2.isOnFrustum(camFrustum, transform);
    }
};

#endif
