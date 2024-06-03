//
// Created by Radek on 26.05.2024.
//

#ifndef ENGINE_STATEMACHINE_H
#define ENGINE_STATEMACHINE_H

#include "ECS/Entity.h"
#include "State.h"

class StateMachine : public Component {
    Animator* machineAnimator;
public:
    explicit StateMachine(const State &currentState);
    StateMachine() {};

    void changeState(State newState);
    void awake() override;
    void update() override;

protected:
    State currentState = State();
};


#endif //ENGINE_STATEMACHINE_H
