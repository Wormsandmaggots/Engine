//
// Created by Radek on 10.04.2024.
//

#ifndef ENGINE_COLLIDERCOMPONENT_H
#define ENGINE_COLLIDERCOMPONENT_H

#include "ECS/Component.h"
#include "Physics/Colliders/Collider.h"

class ColliderComponent : public Component{
public:
    ColliderComponent() = default;
    //~ColliderComponent() override;

    void start() override;
    virtual void onCollision(ColliderComponent*);
    virtual void onCollisionExit(ColliderComponent*);
    virtual void onCollisionStart(ColliderComponent*);

    void awake() override{};
    void onDestroy() override{};
    void update() override;
    //void convertToYaml(YAML::Emitter &) override;

    Collider *getCollider() const;

    //void drawEditor() override;

private:
    Collider* collider = new Collider(this);
};


#endif //ENGINE_COLLIDERCOMPONENT_H
