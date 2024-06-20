#pragma once

#include "ECS/Entity.h"
#include "Generative-System/SongAnalizer.h"

class ForwardMovement : public Entity{
public:
    ForwardMovement(const char* filename, glm::vec3 start, glm::vec3 _finish){
        songLength = SongAnalizer::songLenth(filename);
        distance = _finish.z - start.z;
        velocity = distance/songLength;
        globalVelocity = velocity;
        finish = _finish;

    }
    void update() override{
        if(deltaTime<1){
            this->getTransform()->translate(glm::vec3(0.0f,0.0f,velocity*deltaTime));
        }
        //Entity::update();
        if(this->getTransform()->getPosition().z >=finish.z){
            globalVelocity = 0.0f;
        }
        LOG_INFO(std::to_string(this->getTransform()->getPosition().z ));
    }
    float velocity;
private:
    float distance;
    float songLength;
    glm::vec3 finish;
};