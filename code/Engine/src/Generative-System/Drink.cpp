#include "Generative-System/Drink.h"
#include "ScoreNumbers.h"

Drink::Drink(const std::string name, const glm::vec3 &position, Model *model) : Collectable(name, position, model)
{
	this->getTransform()->setScale(glm::vec3(.5));
}

void Drink::update()
{

	if (active)
	{
		if (deltaTime < 1)
			position.z -= speed * deltaTime;
		this->getTransform()->setPosition(position);
	}
	Entity::update();
}

void Drink::onTriggerStay(ColliderComponent *collidedWith)
{
}

void Drink::onTriggerEnter(ColliderComponent *collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftHandPointer" || collidedWith->parentEntity->getName() == "rightHandPointer" || collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer")
	{
		this->animationPos = position;
		this->animation = true;
		ScoreNumbers::getInstance().calculateViewPos(position, "FREEZE!");
		position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		currentDrink = (DrinkType)(rand() % 3);
		combo += 1;
		this->active = false;
		AudioManager::getInstance().playSound("res/content/sounds/effects/drink.wav", 0.4);
	}
}
