#pragma once
#include <vector>

class Shader; 
enum DrinkType {
	InverseInput,
	UpsideDown,
	Drunk,
	None
};

extern std::vector<Shader*> globalShaders;
extern float deltaTime;
extern int score;
extern int combo;
extern bool start;
extern DrinkType currentDrink;
extern float ballSpeed;
extern float songInterval;
extern float forwardSpeed;