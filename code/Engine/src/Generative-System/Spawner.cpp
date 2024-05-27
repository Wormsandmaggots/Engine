#include "Generative-System/Spawner.h"
#include "Generative-System/Ball.h"
#include "Scene/Scene2.h"
#include "Core/AssetManager/AssetManager.h"

Spawner::Spawner(Scene2* scene) {
	this->scene = scene;
	this->prefabBall = nullptr;
	this->model = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
}

void Spawner::spawnBall(const std::string& name, glm::vec3 pos,Model* model) {
	
		Ball* kula = new Ball(name, pos, new Model(*model));
		balls.push_back(kula);
		kula->addComponent(model);
		this->scene->addEntity(kula);
		
}

void Spawner::updateBalls(float dtime) {
	for (Ball* ball : balls) {
		ball->update(dtime);
	}
}

void Spawner::removeBalls(float dtime)
{
	for (Ball* ball : balls) {
		if (ball->getTransform()->getLocalPosition().z > 2.0)
		{
			scene->removeEntity(ball);
		}
	}
}
