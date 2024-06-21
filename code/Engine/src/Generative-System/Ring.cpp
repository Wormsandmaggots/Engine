#include "Generative-System/Ring.h"


Ring::Ring(const std::string name, const glm::vec3& position, Model* model) : Collectable(name, position, model)
{
	speed = 0;
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->getTransform()->setScale(glm::vec3(.3));
}


void Ring::update()
{
	if (active) {
		this->getTransform()->setPosition(position);
	}
	else {
		this->getTransform()->setPosition(glm::vec3(-100));

	}
	Entity::update();
}

void Ring::onTriggerStay(ColliderComponent* collidedWith)
{
}

void Ring::onTriggerEnter(ColliderComponent* collidedWith)
{
}

void Ring::onTriggerExit(ColliderComponent* collidedWith)
{
	
}
