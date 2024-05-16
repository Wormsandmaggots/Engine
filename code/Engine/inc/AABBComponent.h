#ifndef ENGINE_AABBCOMPONENT_H
#define ENGINE_AABBCOMPONENT_H

#include "ECS/Component.h"
#include "AABB.hpp"
#include "LineRenderer.h"

class AABBComponent : public Component
{
public:
    CPM_GLM_AABB_NS::AABB aabb;

    AABBComponent() = default;

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
};

#endif
