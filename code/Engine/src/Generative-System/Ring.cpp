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
	Entity::update();
}

void Ring::onTriggerStay(ColliderComponent* collidedWith)
{
}

void Ring::onTriggerEnter(ColliderComponent* collidedWith)
{
	

	/*float ballPos = collidedWith->parentEntity->getTransform()->getPosition().z;
	if (ballPos < -1.5) {
		position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		this->active = false;
	}*/
}

void Ring::onTriggerExit(ColliderComponent* collidedWith)
{
	if (collidedWith->parentEntity->getName() == "handOrb" || collidedWith->parentEntity->getName() == "footOrb" || collidedWith->parentEntity->getName() == "badOrb" || collidedWith->parentEntity->getName() == "drink") {
		position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		this->active = false;
	}
}

//spwan w pozycji orba
//despawn w momencie przekroczenia orba tresholdu
//despawn w momencie trafienia raczka w orba
