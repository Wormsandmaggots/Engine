#include "Generative-System/Drink.h"

Drink::Drink(const std::string name, const glm::vec3& position, Model* model): Collectable(name, position, model)
{
	type = (DrinkType)(rand() % 3);
	this->getTransform()->setScale(glm::vec3(.001));

}

void Drink::update()
{

	if (active) {
		position.z -= speed * deltaTime;
		this->getTransform()->setPosition(position);
	}
	Entity::update();
	if (!active) {
		
	}
	

}

void Drink::onTriggerStay(ColliderComponent* collidedWith)
{
	
}

void Drink::onTriggerEnter(ColliderComponent* collidedWith)
{
	if (collidedWith->parentEntity->getName() == "leftHandPointer" || collidedWith->parentEntity->getName() == "rightHandPointer" || collidedWith->parentEntity->getName() == "leftFootPointer" || collidedWith->parentEntity->getName() == "rightFootPointer")
	{
		position = glm::vec3(100);
		this->getTransform()->setPosition(glm::vec3(100));
		currentDrink = (DrinkType)(rand() % 3);
		this->active = false;
		// drinkEnable = true;
		//AudioManager::getInstance().playSound("res/content/sounds/effects/clap.wav"); // DZWIEK DO PODMIANY
	}
}
