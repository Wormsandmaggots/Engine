#include "Globals.h"

enum DrinkType;
std::vector<Shader*> globalShaders;
float deltaTime = 0.0f;
int score = 0;
int combo = 0;
bool start = false;
DrinkType currentDrink = DrinkType::None; // Corrected the variable name here
float ballSpeed = 6.0f;