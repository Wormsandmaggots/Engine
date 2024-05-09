#ifndef ENGINE_AABBCOMPONENT_H
#define ENGINE_AABBCOMPONENT_H
#include "ECS/Component.h"
#include "glm/glm.hpp"
#include "AABB.hpp"

class AABBComponent : public AABB, public Component {
public:

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
};

#endif
