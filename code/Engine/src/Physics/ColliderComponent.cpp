//
// Created by Radek on 10.04.2024.
//

#include "Physics/ColliderComponent.h"
#include "Debug/Logger.h"
#include "Scene/Transform2.h"
#include "glm/gtc/type_ptr.hpp"

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
}

void ColliderComponent::onCollisionStart(ColliderComponent *) {
}

Collider *ColliderComponent::getCollider() const 
{
        return collider;
}

void ColliderComponent::convertToYaml(YAML::Emitter &emitter) {
    float* off = glm::value_ptr(collider->getColliderShape()->offset);
    emitter << YAML::Key << "Type" << YAML::Value << "ColliderComponent";
    emitter << YAML::Key << "ColliderType" << YAML::Value << collider->getColliderType();
    emitter << YAML::Key << "Offset" << YAML::Flow << YAML::BeginSeq << off[0] << off[1] << off[2] << YAML::EndSeq;

    if(collider->getColliderShape()->getType() == ColliderShapes::ColliderShapeType::SPHERE)
    {
        emitter << YAML::Key << "Radius" << YAML::Value << collider->getColliderShape()->getRadius();
    }
    else
    {
        float* size = glm::value_ptr(collider->getColliderShape()->size);
        emitter << YAML::Key << "Size" << YAML::Flow << YAML::BeginSeq  << size[0] << size[1] << size[2] << YAML::EndSeq;
    }
}

//ColliderComponent::~ColliderComponent() {
//    delete collider;
//}
