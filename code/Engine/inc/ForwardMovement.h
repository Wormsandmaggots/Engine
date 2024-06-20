#pragma once

#include "ECS/Entity.h"
#include "Generative-System/SongAnalizer.h"

class ForwardMovement : public Entity{
public:
    ForwardMovement(const char* filename, glm::vec3 start, glm::vec3 finish){
        songLength = SongAnalizer::songLenth(filename);
        distance = finish.z - start.z;
        velocity = distance/songLength;

    }
    void update() override{
        this->getTransform()->translate(glm::vec3(0.0f,0.0f,velocity*deltaTime));
    }
    float velocity;
private:
    float distance;
    float songLength;
};