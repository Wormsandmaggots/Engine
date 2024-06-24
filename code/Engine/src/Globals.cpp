#include "Globals.h"

enum DrinkType;
std::vector<Shader*> globalShaders;
float deltaTime = 0.0f;
int score = 0;
int combo = 0;
bool start = false;
DrinkType currentDrink = DrinkType::None;// typ pp i efekt drinka
float ballSpeed = 4.0f;// predkosc orba
float songInterval = 2.0f; // czas sampolowania do systemuGeneratywnego
float forwardSpeed = 0.f; // predkosc w kierunku baru
float freqIncrement = 0.1; // spawnuje sie wiecej orbow z czasem i to jest rate orb/sec 
float orbDistance = 5.0f; // droga do spawnera do gracza
float lastUpdateTime = 0.0f;
float resizeInterval = 1.0f; // Co sekundę
float resizeAmount = 0.01f;
int lastScore = score;
float globalVelocity = 0.0f;
int incrementScore = 100;
std::string pathToSong = "res/content/sounds/songs/00/dream.wav";
float lookatAngle = 1.0f;
int badOrbChance = 5; // w procentach szansa na zly orb

