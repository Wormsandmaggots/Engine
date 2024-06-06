#ifndef ENGINE_DEBUGINPUT_H
#define ENGINE_DEBUGINPUT_H

#include "GLFW/glfw3.h"
#include "Input.h"
#include "Editor/Camera.h"
#include "Debug/Logger.h"


//TODO: Make camera movement on keys continous
class DebugInput {
public:
    //keyboard
    void interpretInput(GLFWwindow *window, Camera& camera, float deltaTime) {
        Input& input = Input::getInstance();

        //movement
        if (input.isKeyPressed(GLFW_KEY_W)) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (input.isKeyPressed(GLFW_KEY_S)) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (input.isKeyPressed(GLFW_KEY_A)) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (input.isKeyPressed(GLFW_KEY_D)) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if (input.isKeyPressed(GLFW_KEY_Q)) {
            camera.ProcessKeyboard(UP, deltaTime);
        }
        if (input.isKeyPressed(GLFW_KEY_E)) {
            camera.ProcessKeyboard(DOWN, deltaTime);
        }

        // functional
        if (input.isKeyPressed(GLFW_KEY_ESCAPE)) {
            //Deltion pending
            LOG_INFO("ESC Pressed");
            glfwSetWindowShouldClose(window, true);
        }
        //shortcuts
        if (input.isShortcutPressed({GLFW_KEY_LEFT_CONTROL, GLFW_KEY_S})) {
            // game saving functionality
            LOG_INFO("CTRL + S Pressed");
        }


        //controller
        if (input.getControllerButtonState(GLFW_JOYSTICK_1, 0) == GLFW_PRESS) {
            LOG_INFO("Square button on the controller is pressed");
        }

        // Mouse
        int state = input.isMouseRightButtonPressed();

        // Jeśli prawy przycisk myszy jest wciśnięty
        if (state == true) {
            if (!camera.canMove) {
                std::tie(lastMouseX, lastMouseY) = input.getCursorPosition(window);
            }

            camera.canMove = true;

            double mouseX, mouseY;
            std::tie(mouseX, mouseY) = input.getCursorPosition(window);

            float xoffset = static_cast<float>(mouseX - lastMouseX);
            float yoffset = static_cast<float>(lastMouseY - mouseY);

            lastMouseX = mouseX;
            lastMouseY = mouseY;

            camera.ProcessMouseMovement(xoffset, yoffset);
        } else {
            camera.canMove = false;
        }

        double scrollOffsetY = input.getScrollOffsetY();

        camera.ProcessMouseScroll(scrollOffsetY);
    }

    int interpretIKInput(GLFWwindow *window, Camera& camera, float deltaTime) {
        Input &input = Input::getInstance();
        ///USED TO DEBUG IK
        if(input.isKeyPressed(GLFW_KEY_O)){
            return -1;
        }
        else if(input.isKeyPressed(GLFW_KEY_P)){
            return 1;
        }
        else{
            return 0;
        }
    }

    float getMouseX() {
        Input& input = Input::getInstance();
        return input.getMouseX();
    }

    float getMouseY() {
        Input& input = Input::getInstance();
        return input.getMouseY();
    }

    std::pair<float, float> getMousePosition(GLFWwindow *window) {
        Input& input = Input::getInstance();
        double mouseX, mouseY;
        std::tie(mouseX, mouseY) = input.getCursorPosition(window);
        return std::make_pair(static_cast<float>(mouseX), static_cast<float>(mouseY));
    }

private:

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

};
#endif