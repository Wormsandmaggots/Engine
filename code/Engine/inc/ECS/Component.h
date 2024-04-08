//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H
#include "Scene/Loader/ISerializable.h"

class Entity;
class Transform2;
class Component : public ISerializable{
public:
    Transform2* parentTransform;
    Component() = default;
    virtual ~Component() = default;
    //when initialized
    virtual void awake() = 0;
    //next frame after initialization
    virtual void start() = 0;
    //every frame
    virtual void update() = 0;
    //when should be destroyed
    virtual void onDestroy() = 0;
    virtual void setParent(Entity*) = 0;
    virtual void setTransform(Transform2*) = 0;
};
#endif //ENGINE_COMPONENT_H
