#ifndef ENGINE_DEBUGINPUT_H
#define ENGINE_DEBUGINPUT_H

#include "GLFW/glfw3.h"
#include "Input.h"
#include "Editor/Camera.h"

class DebugInput {
public:
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
        //functional
        //TODO:Chceck why not working and switch B for ESC
        if (input.wasPressedLastFrame(GLFW_KEY_B)) {
            LOG_INFO("ESC Pressed");
            glfwSetWindowShouldClose(window, true);
        }
        //shortcuts
        //TODO:Switch Z for S
        if (input.isShortcutPressed({GLFW_KEY_LEFT_CONTROL, GLFW_KEY_Z})) {
            // Zapisz grę
            LOG_INFO("CTRL + S Pressed");
        }
    }

};
#endif
