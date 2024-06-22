#pragma once

#include "ECS/Entity.h"
#include "Generative-System/Spawner.h"
#include "Renderer/MaterialAsset.h"
#include "Physics/ColliderComponent.h"
#include "Audio/Sound.h"
#include "Audio/AudioManager.h" // Add this include statement

class BadBall : public Entity {
	glm::vec3 position;
	float speed = ballSpeed;
	bool toDestroy = false;
public:

	bool getToDestory() {
		return toDestroy;
	}

	void onTriggerEnter(ColliderComponent* entity) {
		if (entity->parentEntity->getName() == "leftHandPointer" || entity->parentEntity->getName() == "rightHandPointer" || entity->parentEntity->getName() == "leftFootPointer" || entity->parentEntity->getName() == "rightFootPointer") {
			this->getTransform()->setPosition(glm::vec3(100));
			this->toDestroy = true;
			score -= 100;
			combo = 0;
			position = glm::vec3(100);
			AudioManager::getInstance().playSound("res/content/sounds/effects/fail1.mp3", 1);

		}
	}
	void onTriggerStay(ColliderComponent*) {

	};
	void onTriggerExit(ColliderComponent*) {

	};



	BadBall(const std::string name, const glm::vec3& position, Model* model) : Entity(name), position(position) {
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

		position.z -= speed * deltaTime;
		if (position.z < -1.5 && !toDestroy) {
			toDestroy = true;
			position = glm::vec3(100);
		}
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