#pragma once 
#include "Model.h"
#include <vector>
#include <string>

class Ball;
class Drink;
class Scene2;
class Sound;

class Spawner {


public:
	Scene2* scene;
	Model* ballModel;
	Model* drinkModel;
	Spawner(Scene2* scene);
	std::vector<Ball*> balls;
	std::vector<Drink*> drinks;
	void spawnBall(const std::string& name, glm::vec3 pos,Sound* success,Sound* failure);
	void spawnDrink(const std::string& name, glm::vec3 pos, Sound* success, Sound* failure);
};
