#ifndef ENGINE_PLAYERINPUT_H
#define ENGINE_PLAYERINPUT_H
#include "Input.h"
#include "Debug/Logger.h"
#include "glm/glm.hpp"

class PlayerInput{
public:
    Input& input = Input::getInstance();

    PlayerInput(int joystickId) : joystickId(joystickId), input(Input::getInstance()) {
        input.initializeController(joystickId);
    }

    void interpretInput() {
        for (int button = 0; button < 4; ++button) {
            int buttonState = input.getControllerButtonState(joystickId, button);
            if (buttonState == GLFW_PRESS) {
                switch (button) {
                    case 0: //square/A
                        //LOG_INFO("Joystick " + std::to_string(joystickId) + ": Button Square is pressed"); //A
                        break;
                    case 1: //cross/B
                        //LOG_INFO("Joystick " + std::to_string(joystickId) + ": Button Cross is pressed"); //B
                        break;
                    case 2: //circle/X
                        //LOG_INFO("Joystick " + std::to_string(joystickId) + ": Button Circle  is pressed"); //X
                        break;
                    case 3: //triangle/Y
                        //LOG_INFO("Joystick " + std::to_string(joystickId) + ": Button Triangle is pressed"); //Y
                        break;
                }
            }
        }

        // Interpret joystick state
        for (int axis = 0; axis < 2; ++axis) {
            glm::vec2 position = input.getControllerJoystickState(joystickId, axis);
            if(axis == 0){joystick1 = position;}
            else {joystick2 = position;}
            std::string joystickSide = (axis == 0) ? "Left" : "Right";
            //LOG_INFO("Joystick ID: " + std::to_string(joystickId) + ", " + joystickSide + " Joystick Position: x=" + std::to_string(position.x) + ", y=" + std::to_string(position.y));
        }
        // Interpret trigger state
        for (int trigger = 0; trigger < 2; ++trigger) {
            float triggerValue = input.getControllerTriggerState(joystickId, trigger);
            //std::string triggerSide = (trigger == 0) ? "Left" : "Right";
            //LOG_INFO("Joystick ID: " + std::to_string(joystickId) + ", " + triggerSide + " Trigger Value: " + std::to_string(triggerValue));
        }
    }
    glm::vec2 getJoystick(int num){
        if(num == 1){return joystick1;}
        else if(num == 2) {return joystick2;}
        else {return glm::vec2(0,0);}
    }
private:
    //int joystick;
    int joystickId;
    //Input& input;
    glm::vec2 joystick1;
    glm::vec2 joystick2;
};
#endif
