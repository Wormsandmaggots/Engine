﻿#include "Globals.h"

enum DrinkType;
std::vector<Shader*> globalShaders;
float deltaTime = 0.0f;
int score = 0;
int combo = 0;
bool start = false;
DrinkType currentDrink = DrinkType::None;// typ pp i efekt drinka
float ballSpeed = 3.8f;// predkosc orba
float songInterval = 2.0f; // czas sampolowania do systemuGeneratywnego
float forwardSpeed = 0.f; // predkosc w kierunku baru
float freqIncrement = 0.0; // spawnuje sie wiecej orbow z czasem i to jest rate orb/sec 
float orbDistance = 8.5f; // droga do spawnera do gracza
float lastUpdateTime = 0.0f;

//resizableBar
float resizeInterval = 1.0f; // Co sekundę
//plz do not use 0.001 or something with one at the end for resizeAmount
float resizeAmount = 0.005f;
float timeLeft = 2.0f; // Czas na podbicie paska
bool isCounting = false; // Czy odliczamy czas

int lastScore = score;
float globalVelocity = 0.0f;
int incrementScore = 100;
std::string pathToSong = "res/content/sounds/songs/00/vengeance.wav";
float lookatAngle = 1.0f;
int badOrbChance = 5; // w procentach szansa na zly orb

bool reset = false;
float songLenghtGlobal = 0.0f;
bool lost = false;
bool won = false;
