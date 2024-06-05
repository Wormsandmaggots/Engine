#ifndef ENGINE_BUTTON_H
#define ENGINE_BUTTON_H

#include "HUD/Image.h"
#include <iostream>

class Button : public Image {
public:
    Button(Shader* shader) : Image(shader) {}

    void checkHover(float mouseX, float mouseY) {
        glm::vec3 position = parentTransform->getLocalPosition();
        glm::vec3 scale = parentTransform->getLocalScale();

        if (mouseX >= position.x - scale.x / 2 && mouseX <= position.x + scale.x / 2 &&
            mouseY >= position.y - scale.y / 2 && mouseY <= position.y + scale.y / 2) {
            std::cout << "hover" << std::endl;
        }
    }
};

#endif