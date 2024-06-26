#include "Generative-System/FootOrb.h"
#include "ScoreNumbers.h"
FootOrb::FootOrb(const std::string name, const glm::vec3 &position, Model *model) : Collectable(name, position, model)
{
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->getTransform()->setScale(glm::vec3(.3));
}

void FootOrb::update()
{
	float distanceToPlayer = this->getParent()->getTransform()->getPosition().z - position.z;
	if (active)
	{
		if (deltaTime < 1)
			position.z -= speed * deltaTime;
		this->getTransform()->setPosition(position);
	}
	Entity::update();
}

void FootOrb::onTriggerStay(ColliderComponent *collidedWith)
{
}

void FootOrb::onTriggerEnter(ColliderComponent *collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer")
	{
		this->animationPos = position;
		this->animation = true;
		ScoreNumbers::getInstance().calculateViewPos(position, "100");
		position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		score += 100;
		combo += 1;
		AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 1);
		this->active = false;
	}
}
