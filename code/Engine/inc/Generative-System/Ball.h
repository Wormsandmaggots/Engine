#pragma once

#include "ECS/Entity.h"
#include "Generative-System/Spawner.h"
#include "Renderer/MaterialAsset.h"

class Ball : public Entity {
	glm::vec3 position;
	float speed = 7.5;

public:
	
	Ball(const std::string name, const glm::vec3& position,Model* model): Entity(name),position(position){
		this->getTransform()->setScale(glm::vec3(.3));
		this->getTransform()->setPosition(position);
		this->addComponent(model);
	}

	void update(float time) {
		position.z += speed*time;
		this->getTransform()->setPosition(position);
		Renderer::Render(this->getComponent<Model>());
	}


	

};