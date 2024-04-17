//
// Created by Radek on 08.04.2024.
//

#include "Physics/Colliders/Collider.h"
#include "Physics/CollisionDetection.h"
#include "Physics/CollisionManager.h"
#include "Physics/ColliderComponent.h"

using namespace ColliderShapes;

Collider::Collider(ColliderShapes::Shape *colliderShape, ColliderType colliderType) : colliderType(colliderType) {
    if(colliderShape != nullptr)
    {
        this->colliderShape = colliderShape;
    }
    CollisionManager::AddCollider(this);
}

Collider::~Collider() {
    delete colliderShape;

    CollisionManager::RemoveCollider(this);
}

bool Collider::checkCollision(const Collider &other) {

    ColliderShapeType first = colliderShape->getType();
    ColliderShapeType second = other.colliderShape->getType();

    if(first == second)
    {
        if(first == ColliderShapes::ColliderShapeType::BOX)
        {
            return CollisionDetection::RectangleRectangleCollision((Box*)colliderShape, (Box*)other.colliderShape);
        }
        else
        {
            return CollisionDetection::CircleCircleCollision((Sphere*)colliderShape, (Sphere*)other.colliderShape);
        }
    }
    else
    {
        if(first == ColliderShapeType::BOX)
        {
            return CollisionDetection::RectangleCircleCollision((Box*)colliderShape, (Sphere*)other.colliderShape);
        }
        else
        {
            return CollisionDetection::RectangleCircleCollision((Box*)other.colliderShape, (Sphere*)colliderShape);
        }
    }
}

void Collider::setOnCollision(std::function<void(ColliderComponent*)> newOnCollision) {
    onCollision = newOnCollision;
}

void Collider::setOwner(ColliderComponent *newOwner) {
    owner = newOwner;
}

ColliderComponent *Collider::getOwner() const {
    return owner;
}

const std::function<void(ColliderComponent*)> &Collider::getOnCollision() const {
    return onCollision;
}

Shape *Collider::getColliderShape() const {
    return colliderShape;
}

const std::function<void(ColliderComponent *)> &Collider::getOnCollisionExit() const {
    return onCollisionExit;
}

void Collider::setOnCollisionExit(const std::function<void(ColliderComponent *)> &onCollisionExit) {
    this->onCollisionExit = onCollisionExit;
}

void Collider::setOnCollisionStart(const std::function<void(ColliderComponent *)> &onCollisionStart) {
    this->onCollisionStart = onCollisionStart;
}

const std::function<void(ColliderComponent *)> &Collider::getOnCollisionStart() const {
    return onCollisionStart;
}

ColliderType Collider::getColliderType() const {
    return colliderType;
}

void Collider::setColliderType(ColliderType colliderType) {
    Collider::colliderType = colliderType;
}

void Collider::setColliderShape(Shape *colliderShape) {
    delete this->colliderShape;
    this->colliderShape = colliderShape;
}

Collider::Collider(ColliderComponent *owner) {
    this->owner = owner;

    setOnCollisionStart([this](ColliderComponent* c){this->owner->onCollisionStart(c);});
    setOnCollision([this](ColliderComponent* c){this->owner->onCollision(c);});
    setOnCollisionExit([this](ColliderComponent* c){this->owner->onCollisionExit(c);});

    CollisionManager::AddCollider(this);
}

//void Collider2::setOnCollision(void (ColliderComponent::*func)(ColliderComponent *)) {
//    onCollision = func;
//}
