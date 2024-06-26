#include "Generative-System/HandOrb.h"
#include "ScoreNumbers.h"

HandOrb::HandOrb(const std::string name, const glm::vec3 &position, Model *model) : Collectable(name, position, model)
{
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->getTransform()->setScale(glm::vec3(.3));
}

void HandOrb::update()
{
	if (active)
	{
		if (deltaTime < 1)
			position.z -= speed * deltaTime;
		this->getTransform()->setPosition(position);
	}

	Entity::update();
}

void HandOrb::onTriggerStay(ColliderComponent *collidedWith)
{
}

void HandOrb::onTriggerEnter(ColliderComponent *collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftHandPointer" || collidedWith->parentEntity->getName() == "rightHandPointer")
	{
		this->animationPos = position;
		this->animation = true;
		ScoreNumbers::getInstance().calculateViewPos(position, std::to_string(incrementScore));
		position = glm::vec3(-100);
		this->getTransform()->setPosition(glm::vec3(-100));
		this->active = false;
		score = score + incrementScore * (1.00 + combo * 0.05);
		combo += 1;
		AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 0.4);
	}
}
