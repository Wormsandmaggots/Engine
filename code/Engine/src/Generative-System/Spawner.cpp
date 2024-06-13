#include "Generative-System/Spawner.h"
#include "Generative-System/Ball.h"
#include "Generative-System/Drink.h"
#include "Scene/Scene2.h"
#include "Core/AssetManager/AssetManager.h"

Spawner::Spawner(Scene2* scene) {
	this->scene = scene;
	this->ballModel = new Model("res/content/models/orb2/orb2.fbx");
	this->drinkModel = new Model("res/content/models/kieliszki/drink1/drink1.fbx");
	ballsSpawned=0;
}

void Spawner::spawnBall(const std::string& name, glm::vec3 pos) {
		Ball* kula = new Ball(name, pos, new Model(*this->ballModel));
		balls.push_back(kula);
		kula->getTransform()->setRotation(glm::vec3(90,90,90));
		this->scene->addEntity(kula);
		ballsSpawned++;
}

void Spawner::spawnDrink(const std::string& name, glm::vec3 pos)
{
	Drink* drink = new Drink(name, pos, new Model(*this->drinkModel));
	drinks.push_back(drink);
	drink->getTransform()->setScale(glm::vec3(0.001f));
	this->scene->addEntity(drink);

}





