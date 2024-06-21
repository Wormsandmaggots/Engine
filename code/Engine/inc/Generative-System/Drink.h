#pragma once
#include "Generative-System/Collectable.h"

class Drink : public Collectable {

public:
	DrinkType type;
	Drink(const std::string name, const glm::vec3& position, Model* model);
	void update() override;
	void onTriggerStay(ColliderComponent* collidedWith) override;
	void onTriggerEnter(ColliderComponent* collidedWith) override;
	void onTriggerExit(ColliderComponent* collidedWith) override {};
};