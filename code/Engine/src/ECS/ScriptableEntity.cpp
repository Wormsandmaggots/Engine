//
// Created by Radek on 26.05.2024.
//

#include "ECS/ScriptableEntity.h"
#include <typeinfo>

ScriptableEntity::ScriptableEntity(const string& name) : Entity(name) {}

ScriptableEntity::ScriptableEntity(const Entity& entity) : Entity(entity) {}

void ScriptableEntity::update() {
    Entity::update();
}

void ScriptableEntity::addComponent(Component* c) {
    Entity::addComponent(c);

    if (strcmp(typeid(*c).name(), typeid(ColliderComponent).name()) == 0)
    {
        Collider* addedCollider = dynamic_cast<ColliderComponent*>(c)->getCollider();

        addedCollider->setOnCollision([this](ColliderComponent* collider) { onTriggerStay(collider); });
        addedCollider->setOnCollisionStart([this](ColliderComponent* collider) { onTriggerEnter(collider); });
        addedCollider->setOnCollisionExit([this](ColliderComponent* collider) { onTriggerExit(collider); });
    }
}