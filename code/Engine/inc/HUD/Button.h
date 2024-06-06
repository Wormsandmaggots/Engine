#ifndef ENGINE_BUTTON_H
#define ENGINE_BUTTON_H

#include <iostream>
#include "HUD/Image.h"
#include "Input/Input.h"
#include "Input/DebugInput.h"

class Button : public Image {
public:
    Button(Shader* shader) : Image(shader), isPressed(false), onClick([](){}) {}

    void setOnClick(std::function<void()> onClickFunc) {
        onClick = onClickFunc;
    }

    void checkHover(double mouseX, double mouseY, double windowWidth, double windowHeight) {
        // Convert mouse coordinates to screen space
        double screenX = (mouseX / windowWidth) * 2 - 1;
        double screenY = 1 - (mouseY / windowHeight) * 2;

        // Get button bounds
        glm::vec3 position = getTransform()->getLocalPosition();
        glm::vec3 scale = getTransform()->getLocalScale();

        // Check if mouse is within button bounds
        if (screenX >= position.x - scale.x / 2 && screenX <= position.x + scale.x / 2 &&
            screenY >= position.y - scale.y / 2 && screenY <= position.y + scale.y / 2) {
            std::cout << "hover" << std::endl;
        }
    }
/*
    void checkClick(GLFWwindow* handle, double mouseX, double mouseY, double windowWidth, double windowHeight) {
        // Check if mouse button is pressed
        GLFWwindow* window = (GLFWwindow*) handle;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Convert mouse coordinates to screen space
            double screenX = (mouseX / windowWidth) * 2 - 1;
            double screenY = 1 - (mouseY / windowHeight) * 2;

            // Get button bounds
            glm::vec3 position = getTransform()->getLocalPosition();
            glm::vec3 scale = getTransform()->getLocalScale();

            // Check if mouse is within button bounds
            if (screenX >= position.x - scale.x / 2 && screenX <= position.x + scale.x / 2 &&
                screenY >= position.y - scale.y / 2 && screenY <= position.y + scale.y / 2) {
                std::cout << "click" << std::endl;
            }
        }
    }
*/
    void checkClick(GLFWwindow* handle, double mouseX, double mouseY, double windowWidth, double windowHeight) {
        // Check if mouse button is pressed
        GLFWwindow* window = (GLFWwindow*) handle;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            // Convert mouse coordinates to screen space
            double screenX = (mouseX / windowWidth) * 2 - 1;
            double screenY = 1 - (mouseY / windowHeight) * 2;

            // Get button bounds
            glm::vec3 position = getTransform()->getLocalPosition();
            glm::vec3 scale = getTransform()->getLocalScale();

            // Check if mouse is within button bounds
            if (screenX >= position.x - scale.x / 2 && screenX <= position.x + scale.x / 2 &&
                screenY >= position.y - scale.y / 2 && screenY <= position.y + scale.y / 2) {
                onClick();
            }
        }
    }
private:
    Input* input;
    bool isPressed;
    std::function<void()> onClick;
};

#endif