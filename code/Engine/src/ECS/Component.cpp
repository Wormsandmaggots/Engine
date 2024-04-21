//
// Created by Radek on 11.04.2024.
//

#include "ECS/Component.h"
#include "ECS/Entity.h"

void Component::setParent(Entity *e) {
    parentEntity = e;
    setTransform(parentEntity->getTransform());
}


void Component::setTransform(Transform *t) {
    parentTransform = t;
}

void Component::convertToYaml(YAML::Emitter &) {
    
}
