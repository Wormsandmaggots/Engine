#pragma once
#include <glm/glm.hpp>
#include <random>

#include "ECS/Component.h"
#include "Generative-System/Collectable.h"
#include "Generative-System/HandOrb.h"
#include "Generative-System/FootOrb.h"
#include "Generative-System/BadOrb.h"
#include "Generative-System/Drink.h"
#include "Generative-System/Ring.h"
#include "Generative-System/SongAnalizer.h"

#include "Debug/Logger.h"
#include "Model.h"

class SpawnerComponent : public Component {
public:
	glm::vec3 originPos;
    glm::vec3 inactivePos;
    float orbDistance;
	std::vector<Collectable*> entitiesActive;
	std::vector<Collectable*> entitiesInactive;
    Model* handOrbModel;
    Model* footOrbModel;
    Model* badOrbModel;
    Model* drinkModel;
    Model* ringModel;

	float xRH, yRH, xLH, yLH, xRF, yRF, xLF, yLF;
 

    std::vector<SongSample> songData;
    int songDataIndex=0;

    unsigned int entitiesCount;
    unsigned int orbsSpawned;
    float spawnAfter = 1;
    float spawnTimer;
    std::string songPath;

    SpawnerComponent(std::string songPath, glm::vec3 originPos, unsigned int enritiesCount);
    void update() override;
    void start() override;
    void init();

    template<typename T>
    void spawn(glm::vec3 spawnPos);
    Collectable* spawnRing(glm::vec3 spawnPos);

    void activateEntity(Collectable* ent);
    void deactiveEntity(Collectable* ent);

    bool hasXPercentChance(int x) {
        std::random_device rd; // Obtain a random number from hardware
        std::mt19937 gen(rd()); // Seed the generator
        std::uniform_int_distribution<> distr(1, 100); // Define the range

        int chance = distr(gen); // Generate a number between 1 and 100
        return chance <= x; // 20% chance if the number is between 1 and 20
    }
};