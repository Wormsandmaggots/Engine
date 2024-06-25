#pragma once
#include <vector>
#include <string>

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
extern float freqIncrement; 
extern float orbDistance;

extern float lastUpdateTime;
extern float resizeInterval; // Co sekundę
extern float resizeAmount;
extern float timeLeft; // Czas na podbicie paska
extern bool isCounting; // Czy odliczamy czas
extern int lastScore;

extern float globalVelocity;
extern int incrementScore;
extern std::string pathToSong;
extern float lookatAngle;
extern int badOrbChance;
