//
// Created by Radek on 10.04.2024.
//

#include "Physics/ColliderComponent.h"
#include "Debug/Logger.h"
#include "Scene/Transform2.h"
#include "Editor/Gizmos.h"

void ColliderComponent::start() {
    collider->setOwner(this);
    collider->setOnCollision([this](ColliderComponent* collidedWith){
        onCollision(collidedWith);
    });
    collider->setOnCollisionExit([this](ColliderComponent* collidedWith){
        onCollisionExit(collidedWith);
    });
    collider->setOnCollisionStart([this](ColliderComponent* collidedWith){
        onCollisionStart(collidedWith);
    });
}

void ColliderComponent::onCollision(ColliderComponent *another) {
}

void ColliderComponent::update() {
    collider->getColliderShape()->position = parentTransform->getWorldMatrix()[3];
}

void ColliderComponent::onCollisionExit(ColliderComponent *another) {
    LOG_INFO("no collision");
    color = glm::vec3(0,1,0);
}

void ColliderComponent::onCollisionStart(ColliderComponent *) {
    LOG_INFO("collision started");
    color = glm::vec3(1.0f, 0.0f, 0.0f);
}

Collider *ColliderComponent::getCollider() const {
    return collider;
}
