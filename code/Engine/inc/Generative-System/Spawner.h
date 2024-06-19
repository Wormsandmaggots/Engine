#pragma once 
#include "Model.h"
#include <vector>
#include <string>
#include <random>

class BadBall;
class Ball;
class Drink;
class Scene2;
class Sound;
class Ring;
class Entity;
class Spawner {


public:
	Scene2* scene;
	Model* ballModel;
	Model* drinkModel;
	Model* badBallModel;
	Model* ringModel;
	int ballsSpawned;
	Spawner(Scene2* scene);
	std::vector<Ball*> balls;
	std::vector<BadBall*> badballs;
	std::vector<Drink*> drinks;
	std::vector<Ring*> rings;

	std::vector<Entity*> objectPool;
	void init();
	void update();
	void spawnBall(const std::string& name, glm::vec3 pos);
	void spawnBadBall(const std::string& name, glm::vec3 pos);
	void spawnDrink(const std::string& name, glm::vec3 pos);
	bool hasXPercentChance(int x) {
		std::random_device rd; // Obtain a random number from hardware
		std::mt19937 gen(rd()); // Seed the generator
		std::uniform_int_distribution<> distr(1, 100); // Define the range

		int chance = distr(gen); // Generate a number between 1 and 100
		return chance <= x; // 20% chance if the number is between 1 and 20
	}
};
