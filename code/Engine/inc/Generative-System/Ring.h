#pragma once
#include "Generative-System/Collectable.h"

class Ring : public Collectable {
public:
	glm::vec3 parentPos;
	bool collided = false;

	Ring(const std::string name, const glm::vec3& position, Model* model);
	void update() override;
	void onTriggerStay(ColliderComponent* collidedWith) override;
	void onTriggerEnter(ColliderComponent* collidedWith) override;
	void onTriggerExit(ColliderComponent* collidedWith) override;
	void setColor(glm::vec3);


};