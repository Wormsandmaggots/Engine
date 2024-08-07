#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <unordered_map>
#include <vector>
#include "GLFW/glfw3.h"
#include "Debug/Logger.h"
#include <glm/glm.hpp>

class Input {
public:
    static Input& getInstance() {
        static Input instance;
        return instance;
    }

    //clabacks
    void setupInputCallbacks(GLFWwindow* window) {
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            getInstance().keyCallback(window, key, scancode, action, mods);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
            getInstance().mouseCallback(window, xpos, ypos);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
            if (button == GLFW_MOUSE_BUTTON_LEFT) {
                getInstance().mouseLeftButton = action == GLFW_PRESS;
            } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
                getInstance().mouseRightButton = action == GLFW_PRESS;
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
            getInstance().scrollCallback(window, xoffset, yoffset);
        });
    }
    //callbacks end

    //keyboard
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        //to handle a new kwy, at it to this if statement
        //if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_Q || key == GLFW_KEY_E || key == GLFW_KEY_B || key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_ESCAPE || key == GLFW_KEY_P  ||  key == GLFW_KEY_O ) {
            if (action == GLFW_PRESS) {
                keys[key] = true;
                keysReleased[key] = false;
            } else if (action == GLFW_RELEASE) {
                keys[key] = false;
                keysReleased[key] = true;
            }
        //}
    }

    bool isKeyPressed(int key) {
        return keys[key];
    }

    bool isKeyReleased(int key) {
        return !keys[key] && keysReleased[key];
    }

    bool isKeyHeld(int key) {
        return keys[key] && !keysReleased[key];
    }

    bool isShortcutPressed(const std::vector<int>& keys) {
        for (int key : keys) {
            if (!isKeyPressed(key)) {
                return false;
            }
        }
        return true;
    }

    //mouse
    double getMouseX() const {
        return mouseX;
    }

    double getMouseY() const {
        return mouseY;
    }


    std::pair<double, double> getCursorPosition(GLFWwindow* window) const {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        return std::make_pair(x, y);
    }

    void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
        mouseX = xpos;
        mouseY = ypos;
    }

    bool isMouseLeftButtonPressed() const {
        return mouseLeftButton;
    }

    bool isMouseRightButtonPressed() const {
        return mouseRightButton;
    }

    int getMouseState(GLFWwindow* window, int button) const {
        return glfwGetMouseButton(window, button);
    }

    //scroll

    double getScrollY() const {
        return scrollY;
    }

    double getScrollOffsetY() const {
        double offset = scrollY - lastScrollY;
        lastScrollY = scrollY;
        return offset;
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        scrollY += yoffset;
    }

    //controller

    //variables to store the initial joystick positions
    //std::array<std::array<float, 2>, 2> initialJoystickPositions = {{{0.0f, 0.0f}, {0.0f, 0.0f}}};

    void initializeController(int joystick) {
        if (glfwJoystickPresent(joystick)) {
            LOG_INFO("Controller detected");

            // Get the initial joystick positions
            /*for (int axis = 0; axis < 2; ++axis) {
                auto [x, y] = getControllerJoystickState(joystick, axis);
                initialJoystickPositions[axis][0] = x;
                initialJoystickPositions[axis][1] = y;
            }*/
        } else {
            LOG_WARNING("No controller detected");
        }
    }

    //buttons and bumpers
    int getControllerButtonState(int joystick, int button) const {
        int count;
        const unsigned char* buttons = glfwGetJoystickButtons(joystick, &count);
        if (button < count) {
            return buttons[button];
        } else {
            return GLFW_RELEASE;
        }
    }

    //zmien axis na glaska
    //axis 0 - left stick axis 1 - right stick
    //
    glm::vec2 getControllerJoystickState(int joystick, int axis) const {
        GLFWgamepadstate state;
        if (glfwGetGamepadState(joystick, &state)) {
            float x = state.axes[axis * 2];
            float y = state.axes[axis * 2 + 1];

            // Apply dead zone
            float deadZone = 0.1f;
            if (std::abs(x) < deadZone) x = 0.0f;
            if (std::abs(y) < deadZone) y = 0.0f;

            return glm::vec2(x, y);
        } else {
            return glm::vec2(0.0f, 0.0f);
        }
    }


    float getControllerTriggerState(int joystick, int trigger) const {
        GLFWgamepadstate state;
        if (glfwGetGamepadState(joystick, &state)) {
            return state.axes[trigger + 4]; // Triggers are usually axes 4 and 5
        } else {
            return -1.0f;
        }
    }


private:
    //keyboard
    std::unordered_map<int, bool> keys;
    std::unordered_map<int, bool> keysReleased;

    //mouse
    double mouseX = 0.0;
    double mouseY = 0.0;

    bool mouseLeftButton = false;
    bool mouseRightButton = false;

    //scroll
    double scrollY = 0.0;
    mutable double lastScrollY = 0.0;

    Input() {}
    Input(const Input&) = delete;
    void operator=(const Input&) = delete;
};

#endif