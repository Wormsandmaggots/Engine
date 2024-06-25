#pragma once

#include "ECS/Entity.h"
#include "Generative-System/SongAnalizer.h"

class ForwardMovement : public Entity{
public:
    ForwardMovement(std::string _filename, glm::vec3 _start, glm::vec3 _finish){


        //velocity = 0;

        finish = _finish;
        //filename = _filename;
        startPoint = _start;

    }
    void update() override{

        if(!started){
            start();
            started = true;
        }

        if(deltaTime<1){
            this->getTransform()->translate(glm::vec3(0.0f,0.0f,velocity*deltaTime));
        }
        //Entity::update();
        if(this->getTransform()->getPosition().z >=finish.z){
            globalVelocity = 0.0f;
        }
    }

    void start (){
        songLength = SongAnalizer::songLenth(pathToSong);
        distance = finish.z - startPoint.z;
        velocity = distance / songLength;
        forwardSpeed = velocity;
        globalVelocity = velocity;
        songLenghtGlobal = songLength;
    }
    float velocity;
private:
    float distance;
    float songLength;
    glm::vec3 finish;
    //std::string filename;
    glm::vec3 startPoint;
    bool started = false;
};