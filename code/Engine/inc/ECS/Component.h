//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

#include "yaml-cpp/emitter.h"

class Entity;
class Transform2;
class Component {
public:
    Transform2* parentTransform;
    Entity* parentEntity;
    Component() = default;
    virtual ~Component() = default;
    //when initialized
    virtual void awake() {};
    //next frame after initialization
    virtual void start() {};
    //every frame
    virtual void update() {};
    //when should be destroyed
    virtual void onDestroy() {};
    virtual void setParent(Entity* e);
    virtual void setTransform(Transform2* t);
    virtual void convertToYaml(YAML::Emitter&);
    virtual void drawEditor() {};

private:

};
#endif //ENGINE_COMPONENT_H
