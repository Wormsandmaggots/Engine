#include "Generative-System/Spawner.h"
#include "Generative-System/Ball.h"
#include "Generative-System/Drink.h"
#include "Scene/Scene2.h"
#include "Core/AssetManager/AssetManager.h"

Spawner::Spawner(Scene2* scene) {
	this->scene = scene;
	this->ballModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
	this->drinkModel = new Model("res/content/models/kieliszki/drink1/drink1.fbx", new MaterialAsset("res/content/materials/color_white.json"));
}

void Spawner::spawnBall(const std::string& name, glm::vec3 pos,Sound* success, Sound* failure) {
		Ball* kula = new Ball(name, pos, new Model(*this->ballModel), success, failure);
		balls.push_back(kula);
		this->scene->addEntity(kula);
}

void Spawner::spawnDrink(const std::string& name, glm::vec3 pos, Sound* success, Sound* failure)
{
	Drink* drink = new Drink(name, pos, new Model(*this->drinkModel), success, failure);
	drinks.push_back(drink);
	drink->getTransform()->setScale(glm::vec3(0.001f));
	this->scene->addEntity(drink);

}





