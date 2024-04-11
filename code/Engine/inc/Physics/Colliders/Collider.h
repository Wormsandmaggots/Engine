//
// Created by Radek on 08.04.2024.
//

#ifndef ENGINE_COLLIDER_H
#define ENGINE_COLLIDER_H

#include <functional>
#include "ColliderShapes.h"

class ColliderComponent;

enum ColliderType{
    COLLIDER,
    TRIGGER
};

class Collider {
public:
    explicit Collider(ColliderShapes::Shape *colliderShape = nullptr, ColliderType colliderType = TRIGGER);
    virtual ~Collider();

    bool checkCollision(const Collider&);

    void setOnCollision(std::function<void(ColliderComponent*)>);
    void setOnCollisionExit(const std::function<void(ColliderComponent *)> &onCollisionExit);
    void setOnCollisionStart(const std::function<void(ColliderComponent *)> &onCollisionStart);
    void setOwner(ColliderComponent*);

    [[nodiscard]] ColliderComponent *getOwner() const;
    [[nodiscard]] const std::function<void(ColliderComponent*)> &getOnCollision() const;
    [[nodiscard]] const std::function<void(ColliderComponent *)> &getOnCollisionExit() const;
    [[nodiscard]] const std::function<void(ColliderComponent *)> &getOnCollisionStart() const;
    [[nodiscard]] ColliderShapes::Shape *getColliderShape() const;

    bool collided = false;
    ColliderComponent* collidedWith;

private:
    ColliderShapes::Shape *colliderShape = new ColliderShapes::Sphere(glm::vec3(0.f), glm::vec3(0.f), 0.7102f);
    ColliderType colliderType;
    ColliderComponent* owner = nullptr;
    std::function<void(ColliderComponent*)> onCollision = nullptr;
    std::function<void(ColliderComponent*)> onCollisionExit = nullptr;
    std::function<void(ColliderComponent*)> onCollisionStart = nullptr;
};


#endif //ENGINE_COLLIDER_H
