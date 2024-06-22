#include "Generative-System/Ring.h"


Ring::Ring(const std::string name, const glm::vec3& position, Model* model) : Collectable(name, position, model)
{
	speed = 0;
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	//this->getComponent<ColliderComponent>()->getCollider()->getColliderShape()->setRadius(1);

}

void Ring::setColor(glm::vec3 color)
{
	this->model->getMaterial()->SetVec4("color", glm::vec4(color, 1));
}

void Ring::update()
{
	if (active) {
		if (this->getParent()->getName() == "badOrb") {
			setColor(glm::vec3(1,0,0));
		}
		else {
			if (collided) {
				setColor(glm::vec3(0, 1, 0));
			}
			else {
				setColor(glm::vec3(1, 1, 1));
			}
		}
		parentPos = this->getParent()->getTransform()->getPosition();
		this->getTransform()->setScale(glm::vec3(0.4*((parentPos.z - position.z + 1.5) / orbDistance)));
		this->getTransform()->setPosition(position);
	}

	Entity::update();
}

void Ring::onTriggerStay(ColliderComponent* collidedWith)
{

}

void Ring::onTriggerEnter(ColliderComponent* collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftHandPointer" || collidedWith->parentEntity->getName() == "rightHandPointer" || collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer") {
		collided = true;
	}
}

void Ring::onTriggerExit(ColliderComponent* collidedWith)// to chyba nie dziala
{

	if (collidedWith->parentEntity->getName() == "leftHandPointer" || collidedWith->parentEntity->getName() == "rightHandPointer" || collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer") {
		collided = false;
	}
}


