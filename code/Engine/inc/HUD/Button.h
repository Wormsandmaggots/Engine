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

    void setAnimationTextures(const std::vector<std::string>& textures) {
        animationTextures = textures;
    }

    void animate() {
        if (animationTextures.empty()) return;
        setTexture(animationTextures[currentTextureIndex]);
        currentTextureIndex = (currentTextureIndex + 1) % animationTextures.size();
    }
/*
    void checkHover(double mouseX, double mouseY, double windowWidth, double windowHeight) {
        double screenX = (mouseX / windowWidth) * 2 - 1;
        double screenY = 1 - (mouseY / windowHeight) * 2;

        glm::vec3 position = getTransform()->getLocalPosition();
        glm::vec3 scale = getTransform()->getLocalScale();

        if (screenX >= position.x - scale.x / 2 && screenX <= position.x + scale.x / 2 &&
            screenY >= position.y - scale.y / 2 && screenY <= position.y + scale.y / 2) {
            std::cout << "hover" << std::endl;
            animate();
        }
    }
*/
    void checkHover(double mouseX, double mouseY, double windowWidth, double windowHeight) {
        double screenX = (mouseX / windowWidth) * 2 - 1;
        double screenY = 1 - (mouseY / windowHeight) * 2;

        glm::vec3 position = getTransform()->getLocalPosition();
        glm::vec3 scale = getTransform()->getLocalScale();

        bool isHovered = screenX >= position.x - scale.x / 2 && screenX <= position.x + scale.x / 2 &&
                         screenY >= position.y - scale.y / 2 && screenY <= position.y + scale.y / 2;

        if (isHovered) {
            std::cout << "hover" << std::endl;
            animate();
        } else {
            resetAnimation();
        }
    }

    void resetAnimation() {
        currentTextureIndex = 0;
        if (!animationTextures.empty()) {
            setTexture(animationTextures[currentTextureIndex]);
        }
    }

    void checkClick(GLFWwindow* handle, double mouseX, double mouseY, double windowWidth, double windowHeight) {
        GLFWwindow* window = (GLFWwindow*) handle;
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double screenX = (mouseX / windowWidth) * 2 - 1;
            double screenY = 1 - (mouseY / windowHeight) * 2;

            glm::vec3 position = getTransform()->getLocalPosition();
            glm::vec3 scale = getTransform()->getLocalScale();

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

    std::vector<std::string> animationTextures;
    int currentTextureIndex = 0;
};

#endif