#pragma once

#include "ECS/Entity.h"
#include "Generative-System/Spawner.h"
#include "Renderer/MaterialAsset.h"
#include "Physics/ColliderComponent.h"
#include "Audio/Sound.h"

class Ball : public Entity {
	glm::vec3 position;
	float speed = 5.1;
	bool toDestroy = false;
	Sound* success;
	Sound* failure;
	

public:
	
	bool getToDestory() {
		return toDestroy;
	}

	void onTriggerEnter(ColliderComponent* entity) {
		if (entity->parentEntity->getName() == "leftHandPointer" || entity->parentEntity->getName() == "rightHandPointer") {
			this->getComponent<Model>()->getMaterial()->SetVec4("color", glm::vec4(0, 1, 0, 1));
			this->getTransform()->setPosition(glm::vec3(100));
			this->toDestroy = true;
			score += 100;
			position = glm::vec3(100);
			success->play();

		}
	}
	void onTriggerStay(ColliderComponent*) {
		
	};
	void onTriggerExit(ColliderComponent*) {
	
	};



	Ball(const std::string name, const glm::vec3& position,Model* model,Sound* success,Sound* failure): Entity(name),position(position){
		this->success = success;
		this->failure = failure;
		
		ColliderComponent* collider = new ColliderComponent();
		collider->setParent(this);
		collider->start();
		this->getTransform()->setScale(glm::vec3(.3));
		this->getTransform()->setPosition(position);
		this->addComponent(model);
		this->addComponent(collider);	

	}

	void update() override{
		
			position.z += speed * deltaTime;
			if (position.z > 10.0 && !toDestroy) {
				toDestroy = true;
				position.z = 100;
				failure->play();
				this->getComponent<Model>()->getMaterial()->SetVec4("color", glm::vec4(1, 0, 0, 1));
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