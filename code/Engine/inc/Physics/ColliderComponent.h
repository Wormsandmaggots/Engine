//
// Created by Radek on 10.04.2024.
//

#ifndef ENGINE_COLLIDERCOMPONENT_H
#define ENGINE_COLLIDERCOMPONENT_H

#include "ECS/Component.h"
#include "Physics/Colliders/Collider.h"

class ColliderComponent : public Component{
public:
    void start() override;
    virtual void onCollision(ColliderComponent*);
    virtual void onCollisionExit(ColliderComponent*);
    virtual void onCollisionStart(ColliderComponent*);

    void awake() override{};
    void onDestroy() override{};
    void update() override;

    Collider *getCollider() const;

    //!TEST ONLY
    glm::vec3 color = glm::vec3(0,1,0);

private:
    Collider* collider = new Collider(nullptr);
};


#endif //ENGINE_COLLIDERCOMPONENT_H
