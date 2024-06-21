#include "Generative-System/FootOrb.h"

FootOrb::FootOrb(const std::string name, const glm::vec3& position, Model* model) : Collectable(name, position, model)
{
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->getTransform()->setScale(glm::vec3(.3));
}

void FootOrb::update()
{
	if (active)
	{
		position.z -= speed * deltaTime;
		/*if (position.z < -1.5) {
			active = false;
			position = glm::vec3(-100);
			combo = 0;
			AudioManager::getInstance().playSound("res/content/sounds/effects/fail1.mp3", 0.1);
		}*/
		this->getTransform()->setPosition(position);
	}
	Entity::update();
}

void FootOrb::onTriggerStay(ColliderComponent* collidedWith)
{
	
}

void FootOrb::onTriggerEnter(ColliderComponent* collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer") {
		position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		score += 100;
		combo += 1;
		AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 1);
		this->active = false;
	}
}
