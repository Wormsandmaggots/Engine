#ifndef ENGINE_DEBUGINPUT_H
#define ENGINE_DEBUGINPUT_H

#include "GLFW/glfw3.h"
#include "Input.h"
#include "Editor/Camera.h"

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
            // Zapisz grę
            LOG_INFO("CTRL + S Pressed");
        }
        /*if (input.wasPressedLastFrame(GLFW_KEY_B)) {
            LOG_INFO("B was pressed in the last frame");
        }*/
        //mouse
        double lastX = input.getMouseX();
        double lastY = input.getMouseY();

        if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)) {
            double xoffset = input.getMouseX() - lastX;
            double yoffset = lastY - input.getMouseY(); // reversed since y-coordinates range from bottom to top
            lastX = input.getMouseX();
            lastY = input.getMouseY();
            camera.ProcessMouseMovement(xoffset, yoffset);
        }

        if (input.isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
            double yoffset = lastY - input.getMouseY(); // reversed since y-coordinates range from bottom to top
            lastY = input.getMouseY();
            camera.ProcessMouseScroll(yoffset);
            camera.canMove = true;
        } else {
            camera.canMove = false;
        }

        double scrollY = input.getScrollY();
        camera.ProcessMouseScroll(scrollY);
    }

    //mouse


};
#endif