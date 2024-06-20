#include "Generative-System/Spawner.h"
#include "Generative-System/Ball.h"
#include "Generative-System/Drink.h"
#include "Generative-System/Ring.h"
#include "Generative-System/BadBall.h"
#include "Scene/Scene2.h"
#include "Core/AssetManager/AssetManager.h"

Spawner::Spawner(Scene2* scene) {
	this->scene = scene;
	this->ballModel = new Model("res/content/models/orb2/orb2.fbx");
	this->ringModel = new Model("res/content/models/orb2/obrys2.fbx");
	this->drinkModel = new Model("res/content/models/kieliszki/drink1/drink1.fbx");
	this->badBallModel = new Model("res/content/models/orbWrong/orbWrong.fbx");
	ballsSpawned=0;
}

void Spawner::spawnBall(const std::string& name, glm::vec3 pos) {
	
	Ball* kula = new Ball(name, pos, new Model(*this->ballModel));
		balls.push_back(kula);
		kula->getTransform()->setRotation(glm::vec3(90,90,90));
		this->scene->addEntity(kula);
		Ring* ring = new Ring(name, glm::vec3(pos.x,pos.y,pos.z-5.3), new Model(*this->ringModel));
		rings.push_back(ring);
		ring->getTransform()->setRotation(glm::vec3(90, 90, 90));
		kula->addChild(ring);
		ballsSpawned++;
}

void Spawner::spawnBadBall(const std::string& name, glm::vec3 pos) {
	BadBall* kula = new BadBall(name, pos, new Model(*this->badBallModel));
	badballs.push_back(kula);
	kula->getTransform()->setRotation(glm::vec3(90, 90, 90));
	this->scene->addEntity(kula);
	ballsSpawned++;
	Ring* ring = new Ring(name, glm::vec3(pos.x, pos.y, pos.z-5.3), new Model(*this->ringModel));
	rings.push_back(ring);
	ring->getTransform()->setRotation(glm::vec3(90, 90, 90));
	kula->addChild(ring);
}

void Spawner::spawnDrink(const std::string& name, glm::vec3 pos)
{
	Drink* drink = new Drink(name, pos, new Model(*this->drinkModel));
	drinks.push_back(drink);
	drink->getTransform()->setScale(glm::vec3(0.001f));
	this->scene->addEntity(drink);

	Ring* ring = new Ring(name, glm::vec3(pos.x, pos.y, -0.3), new Model(*this->ringModel));
	rings.push_back(ring);
	ring->getTransform()->setRotation(glm::vec3(90, 90, 90));
	drink->addChild(ring);
}

void Spawner::update() {

	for (int i = 0; i < rings.size(); i++) {
		if (rings[i]->getToDestory()) {
			this->scene->removeEntity(rings[i]);
			rings.erase(rings.begin() + i);
		}
	}
	for (int i = 0; i < balls.size(); i++) {

		if (balls[i]->getToDestory()) {
			this->scene->removeEntity(balls[i]);
			balls.erase(balls.begin() + i);
		}
	}
	for (int i = 0; i < badballs.size(); i++) {
		if (badballs[i]->getToDestory()) {
			this->scene->removeEntity(badballs[i]);
			badballs.erase(badballs.begin() + i);
		}
	}
	for (int i = 0; i < drinks.size(); i++) {
		if (drinks[i]->getToDestory()) {
			this->scene->removeEntity(drinks[i]);
			drinks.erase(drinks.begin() + i);
		}
	}
}





