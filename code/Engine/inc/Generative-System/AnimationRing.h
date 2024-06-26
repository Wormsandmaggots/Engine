#pragma once
#include "Generative-System/Collectable.h"

class AnimationRing : public Collectable
{
public:
	glm::vec3 parentPos;
	glm::vec3 scale;
	bool collided = false;

	AnimationRing(const std::string name, const glm::vec3& position, Model* model);
	void update() override;
	void onTriggerStay(ColliderComponent* collidedWith) override;
	void onTriggerEnter(ColliderComponent* collidedWith) override;
	void onTriggerExit(ColliderComponent* collidedWith) override;
	void setColor(glm::vec3);
};