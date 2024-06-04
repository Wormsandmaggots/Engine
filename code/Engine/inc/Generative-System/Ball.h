#pragma once

#include "ECS/Entity.h"
#include "Generative-System/Spawner.h"
#include "Renderer/MaterialAsset.h"
#include "Physics/ColliderComponent.h"

class Ball : public Entity {
	glm::vec3 position;
	float speed = 5.1;
	bool toDestroy = false;
	

public:
	
	bool getToDestory() {
		return toDestroy;
	}

	void onTriggerEnter(ColliderComponent* entity) {
		
		if (entity->parentEntity->getName() != "ball") {
			//this->getTransform()->setPosition(glm::vec3(10,10,10));
			//entity->parentEntity->getTransform()->setPosition(glm::vec3(100));
			//entity->getCollider()->collidedWith->parentEntity->getTransform()->setPosition(glm::vec3(100));
			this->getTransform()->setPosition(glm::vec3(100));
			this->toDestroy = true;
			position = glm::vec3(100);
			LOG_INFO("callision");
		}
	}
	void onTriggerStay(ColliderComponent*) {
		
	};
	void onTriggerExit(ColliderComponent*) {
	
	};



	Ball(const std::string name, const glm::vec3& position,Model* model): Entity(name),position(position){
		ColliderComponent* collider = new ColliderComponent();
		collider->setParent(this);
		collider->start();
		this->getTransform()->setScale(glm::vec3(.3));
		this->getTransform()->setPosition(position);
		this->addComponent(model);
		this->addComponent(collider);	
	}

	void update() override{
		position.z += speed*deltaTime;
		this->getTransform()->setPosition(position);
		
		if (position.z == 5) {
			position = glm::vec3(100);
		}
		
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