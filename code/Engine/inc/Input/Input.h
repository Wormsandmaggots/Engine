#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include <unordered_map>
#include "GLFW/glfw3.h"

class Input {
public:
    static Input& getInstance() {
        static Input instance;
        return instance;
    }

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_W || key == GLFW_KEY_A || key == GLFW_KEY_S || key == GLFW_KEY_D) {
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

    bool wasPressedLastFrame(int key) {
        return keysLastFrame[key] && !keys[key];
    }

    void endFrame() {
        keysLastFrame = keys;
    }

    bool isShortcutPressed(const std::vector<int>& keys) {
        for (int key : keys) {
            if (!isKeyPressed(key)) {
                return false;
            }
        }
        return true;
    }

private:
    std::unordered_map<int, bool> keys;
    std::unordered_map<int, bool> keysReleased;
    std::unordered_map<int, bool> keysLastFrame;

    Input() {}
    Input(const Input&) = delete;
    void operator=(const Input&) = delete;
};

#endif