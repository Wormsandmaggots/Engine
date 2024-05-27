//
// Created by Radek on 26.05.2024.
//

#ifndef ENGINE_STATE_H
#define ENGINE_STATE_H

#include "ECS/Entity.h"

class State {
public:
    State() {}

    virtual void enter(Entity* context) { LOG_WARNING(context->getName()); };
    virtual void update(Entity* context) {};
    virtual void exit(Entity* context) {};

};


#endif //ENGINE_STATE_H
