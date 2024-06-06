#include "Generative-System/Spawner.h"
#include "Generative-System/Ball.h"
#include "Scene/Scene2.h"
#include "Core/AssetManager/AssetManager.h"

Spawner::Spawner(Scene2* scene) {
	this->scene = scene;
	this->model = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
}

void Spawner::spawnBall(const std::string& name, glm::vec3 pos,Sound* success, Sound* failure) {
	
		Ball* kula = new Ball(name, pos, new Model(*this->model),success,failure);
		balls.push_back(kula);
		this->scene->addEntity(kula);
		
}



