#include "Generative-System/AnimationRing.h"


AnimationRing::AnimationRing(const std::string name, const glm::vec3& position, Model* model) : Collectable(name, position, model)
{
	speed = 0;
	this->scale = glm::vec3(0.1);
	this->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->getTransform()->setScale(scale);


}

void AnimationRing::setColor(glm::vec3 color)
{
	this->model->getMaterial()->SetVec4("materialColor", glm::vec4(color, 1));
}

void AnimationRing::update()
{
	if (active) {

		if (this->ringType == "badOrb") {
			setColor(glm::vec3(1,0,0));
		}
		else if (this->ringType == "drink") {
			setColor(glm::vec3(1));
		}
		else if (this->ringType == "handOrb") {
			setColor(glm::vec3(0.8,0.8, 0));
		}
		else if (this->ringType == "footOrb") {
			setColor(glm::vec3(0, 0, 0.8));
		}


		if (scale.z < 0.7) {
			scale += glm::vec3(2*deltaTime);
		}
		else {
			scale = glm::vec3(0.1);
			this->active = false;
		}

		this->getTransform()->setScale(scale);
		this->getTransform()->setPosition(position);
	}

	Entity::update();
}

void AnimationRing::onTriggerStay(ColliderComponent* collidedWith)
{
}

void AnimationRing::onTriggerEnter(ColliderComponent* collidedWith)
{
}

void AnimationRing::onTriggerExit(ColliderComponent* collidedWith)// to chyba nie dziala
{
}


