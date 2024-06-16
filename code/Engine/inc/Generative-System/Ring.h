#pragma once

#include "ECS/Entity.h"
#include "Generative-System/Spawner.h"
#include "Renderer/MaterialAsset.h"
#include "Physics/ColliderComponent.h"
#include "Audio/Sound.h"
#include "Audio/AudioManager.h"

class Ring : public Entity {
	glm::vec3 position;
	bool toDestroy = false;
	bool active = false;
public:

	bool getToDestory() {
		return toDestroy;
	}

	void onTriggerEnter(ColliderComponent* entity) {
		float ballPos = entity->parentEntity->getTransform()->getPosition().z;
		if (ballPos <-1.5) {
			this->toDestroy = true;
			position = glm::vec3(100);
		}
		/*if (entity->parentEntity->getName() == "ball") {
			this->toDestroy = true;
			position = glm::vec3(100);
			LOG_INFO("ball hit ring");
		}*/
	}
	void onTriggerStay(ColliderComponent*) {

	};
	void onTriggerExit(ColliderComponent*) {

	};



	Ring(const std::string name, const glm::vec3& position, Model* model) : Entity(name), position(position) {
		ColliderComponent* collider = new ColliderComponent();
		collider->setParent(this);
		collider->start();
		collider->getCollider()->getColliderShape()->setRadius(0.6);
		this->getTransform()->setScale(glm::vec3(.3));
		this->getTransform()->setPosition(position);
		this->addComponent(model);
		this->addComponent(collider);

	}

	void update() override {
		this->getTransform()->setPosition(position);
		Entity::update();
	}

	void addComponent(Component* c) override {
		Entity::addComponent(c);

		if (strcmp(typeid(*c).name(), typeid(ColliderComponent).name()) == 0)
		{
			Collider* addedCollider = dynamic_cast<ColliderComponent*>(c)->getCollider();

			addedCollider->setOnCollision([this](ColliderComponent* collider) { onTriggerStay(collider); });
			addedCollider->setOnCollisionStart([this](ColliderComponent* collider) { onTriggerEnter(collider); });
			addedCollider->setOnCollisionExit([this](ColliderComponent* collider) { onTriggerExit(collider); });
		}
	}
};