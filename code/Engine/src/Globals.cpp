#include "Globals.h"

enum DrinkType;
std::vector<Shader*> globalShaders;
float deltaTime = 0.0f;
int score = 0;
int incrementScore = 100;
int combo = 0;
bool start = false;
DrinkType currentDrink = DrinkType::None;
float ballSpeed = 6.0f;
float lastUpdateTime = 0.0f;
float resizeInterval = 1.0f; // Co sekundę
float resizeAmount = 0.01f;
int lastScore = score;
float globalVelocity = 0.0f;

