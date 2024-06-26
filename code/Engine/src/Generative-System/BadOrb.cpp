#include "Generative-System/BadOrb.h"
#include "ScoreNumbers.h"

BadOrb::BadOrb(const std::string name, const glm::vec3& position, Model* model) : Collectable(name, position, model)
{
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->getTransform()->setScale(glm::vec3(.3));

}

void BadOrb::update()
{
	if (active) {
        if(deltaTime<1)
		position.z -= speed * deltaTime;
		this->getTransform()->setPosition(position);
	}
	Entity::update();
}

void BadOrb::onTriggerStay(ColliderComponent* collidedWith)
{
	
}

void BadOrb::onTriggerEnter(ColliderComponent* collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftHandPointer" || collidedWith->parentEntity->getName() == "rightHandPointer" || collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer") {
        ScoreNumbers::getInstance().calculateViewPos(position,-100);
        position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		score -= 100;
		combo = 0;
		active = false;
		AudioManager::getInstance().playSound("res/content/sounds/effects/fail1.mp3", 1);
	}
}
