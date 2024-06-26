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
#include "Generative-System/AnimationRing.h"

#include "Debug/Logger.h"
#include "Model.h"

class SpawnerComponent : public Component {
public:
	glm::vec3 originPos;
    glm::vec3 inactivePos;
    //float orbDistance;
	std::vector<Collectable*> entitiesActive;
	std::vector<Collectable*> entitiesInactive;
    Model* handOrbModel;
    Model* footOrbModel;
    Model* badOrbModel;
    Model* drinkModel;
    Model* ringXModel;
    Model* ringUPModel;
    Model* ringDOWNModel;

	float xRH, yRH, xLH, yLH, xRF, yRF, xLF, yLF;
    float orbRadius = 0.3;

    std::vector<SongSample> songData;
    int songDataIndex=0;

    bool started = false;

    unsigned int entitiesCount;
    unsigned int orbsSpawned;
    float spawnAfter = songInterval;
    float spawnTimer;
    std::string songPath;

    SpawnerComponent(std::string songPath, glm::vec3 originPos, unsigned int enritiesCount);
    void update() override;
    void start() override;
    void init();
    void reset();


    template<typename T>
    void spawn(glm::vec3 spawnPos);
    Collectable* spawnRing(glm::vec3 spawnPos,std::string ringType);
    void spawnAnimationRing(glm::vec3 spawnPos, std::string ringType);


    void activateEntity(Collectable* ent);
    void deactiveEntity(Collectable* ent);

    bool hasXPercentChance(int x) {
        std::random_device rd;
        std::mt19937 gen(rd()); 
        std::uniform_int_distribution<> distr(1, 100);

        int chance = distr(gen);
        return chance <= x; 
    }
    void checkAndAdjustForOverlap(float& x1, float& y1, float& x2, float& y2, float radius) {
        glm::vec2 pos1(x1, y1);
        glm::vec2 pos2(x2, y2);
        if (glm::distance(pos1, pos2) < 2 * radius) {
            // Adjust positions to avoid overlap
            glm::vec2 direction = glm::normalize(pos2 - pos1);
            glm::vec2 midpoint = (pos1 + pos2) / 2.0f;
            pos1 = midpoint - direction * radius;
            pos2 = midpoint + direction * radius;
            x1 = pos1.x;
            y1 = pos1.y;
            x2 = pos2.x;
            y2 = pos2.y;
        }
    }

    float generateRandomFloat(float x, float y) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> distr(x, y);
		return distr(gen);
    }
};