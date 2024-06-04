#pragma once 
#include "Model.h"
#include <vector>
#include <string>

class Ball;
class Scene2;

class Spawner {


public:
	Scene2* scene;
	Ball* prefabBall;
	Model* model;
	Spawner(Scene2* scene);
	std::vector<Ball*> balls;
	void spawnBall(const std::string& name, glm::vec3 pos);
	void updateBalls();
	
};
