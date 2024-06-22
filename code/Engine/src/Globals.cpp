#include "Globals.h"

enum DrinkType;
std::vector<Shader*> globalShaders;
float deltaTime = 0.0f;
int score = 0;
int combo = 0;
bool start = false;
DrinkType currentDrink = DrinkType::None;// typ pp i efekt drinka
float ballSpeed = 6.0f;// predkosc orba
float songInterval = 1.0f; // czas sampolowania do systemuGeneratywnego
float forwardSpeed = 0.f; // predkosc w kierunku baru
float freqIncrement = 0.0; // spawnuje sie wiecej orbow z czasem i to jest rate orb/sec 
float orbDistance = 5.0f; // droga do spawnera do gracza