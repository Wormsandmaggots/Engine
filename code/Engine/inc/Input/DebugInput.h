#ifndef ENGINE_DEBUGINPUT_H
#define ENGINE_DEBUGINPUT_H

#include "GLFW/glfw3.h"
#include "Input.h"
#include "Editor/Camera.h"
#include "Debug/Logger.h"

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

        //controller
        if (input.getControllerButtonState(GLFW_JOYSTICK_1, 0) == GLFW_PRESS) {
            LOG_INFO("Square button on the controller is pressed");
        }

        // Mouse
        // Pobieramy stan prawego przycisku myszy
        //int state = input.getMouseState(window, GLFW_MOUSE_BUTTON_RIGHT);
        int state = input.isMouseRightButtonPressed();

        // Jeśli prawy przycisk myszy jest wciśnięty
        if (state == true) {
            // Jeśli to jest pierwsze wciśnięcie przycisku myszy, zainicjuj wartości lastMouseX i lastMouseY
            if (!camera.canMove) {
                std::tie(lastMouseX, lastMouseY) = input.getCursorPosition(window);
            }

            // Ustawiamy flagę canMove na true
            camera.canMove = true;

            // Pobieramy aktualną pozycję myszy
            double mouseX, mouseY;
            std::tie(mouseX, mouseY) = input.getCursorPosition(window);

            // Obliczamy różnicę w pozycji myszy między bieżącą a poprzednią klatką
            float xoffset = static_cast<float>(mouseX - lastMouseX);
            float yoffset = static_cast<float>(lastMouseY - mouseY); // reversed since y-coordinates range from bottom to top

            // Aktualizujemy pozycję myszy dla następnej klatki
            lastMouseX = mouseX;
            lastMouseY = mouseY;

            // Przekazujemy różnicę w pozycji myszy do metody ProcessMouseMovement klasy Camera
            camera.ProcessMouseMovement(xoffset, yoffset);
        } else {
            // Jeśli prawy przycisk myszy nie jest wciśnięty, ustawiamy flagę canMove na false
            camera.canMove = false;
        }

        // Pobieramy offset scrolla
        double scrollOffsetY = input.getScrollOffsetY();

        // Przekazujemy offset scrolla do metody ProcessMouseScroll klasy Camera
        camera.ProcessMouseScroll(scrollOffsetY);
    }

private:

    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

};
#endif