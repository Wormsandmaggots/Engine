//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

#include "yaml-cpp/emitter.h"

class Entity;
class Transform;
class Component {
public:
    Transform* parentTransform;
    Entity* parentEntity;
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
    virtual void setParent(Entity* e);
    virtual void setTransform(Transform* t);
    virtual void convertToYaml(YAML::Emitter&);
    virtual void drawEditor() {};
};
#endif //ENGINE_COMPONENT_H
