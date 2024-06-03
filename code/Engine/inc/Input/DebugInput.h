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
        if (input.isKeyPressed(GLFW_KEY_P)) {
            state += 1;
        }
        if (input.isKeyPressed(GLFW_KEY_O)) {
            state -= 1;
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
        /*if (input.getControllerButtonState(GLFW_JOYSTICK_1, 0) == GLFW_PRESS) {
            LOG_INFO("Square button on the controller is pressed");
        }*/

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
int state = 0;
private:

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

};
#endif