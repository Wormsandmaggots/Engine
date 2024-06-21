#pragma once

#include "ECS/Entity.h"
#include "Physics/ColliderComponent.h"
#include "Audio/AudioManager.h"


class Collectable :public Entity {
public:
    bool active = false;
    float speed = ballSpeed;
    glm::vec3 position;
    Model* model;
    ColliderComponent* collider;
    Collectable(const std::string name, const glm::vec3& position, Model* model);
    void addComponent(Component*)override;
    void setChildInactive();
    virtual void update() override=0;
    virtual void onTriggerStay(ColliderComponent* collidedWith) = 0;
	virtual void onTriggerEnter(ColliderComponent* collidedWith) = 0;
    virtual void onTriggerExit(ColliderComponent* collidedWith) = 0;

};