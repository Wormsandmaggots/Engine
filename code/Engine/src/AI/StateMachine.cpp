//
// Created by Radek on 26.05.2024.
//

#include "AI/StateMachine.h"

void StateMachine::changeState(State newState) {
    currentState.exit(parentEntity);

    currentState = newState;

    currentState.enter(parentEntity);
}

void StateMachine::update() {
    currentState.update(parentEntity);
}

StateMachine::StateMachine(const State &currentState) : currentState(currentState) {
    this->currentState = currentState;
}

void StateMachine::awake() {
    currentState.enter(parentEntity);
}
