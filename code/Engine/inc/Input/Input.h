#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <unordered_map>
#include <vector>
#include "GLFW/glfw3.h"

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
        if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D || key == GLFW_KEY_Q || key == GLFW_KEY_E || key == GLFW_KEY_B || key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_ESCAPE) {
            if (action == GLFW_PRESS) {
                keys[key] = true;
                keysReleased[key] = false;
            } else if (action == GLFW_RELEASE) {
                keys[key] = false;
                keysReleased[key] = true;
            }
        }
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
/*
    // didn't work
    //
    bool wasPressedLastFrame(int key) {
        return keysLastFrame[key] && !keys[key];
    }

    void endFrame() {
        for (const auto& key : keys) {
            keysLastFrame[key.first] = key.second;
        }
    }
*/
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


private:
    //keyboard
    std::unordered_map<int, bool> keys;
    std::unordered_map<int, bool> keysReleased;
    //std::unordered_map<int, bool> keysLastFrame;

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