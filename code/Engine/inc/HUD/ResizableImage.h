#ifndef ENGINE_RESIZABLEIMAGE_H
#define ENGINE_RESIZABLEIMAGE_H

#include "HUD/Image.h"

class ResizableImage : public Image {
public:
    ResizableImage(Shader* shader) : Image(shader) {
        shader->use();
        shader->setFloat("scale", 0.166f);
    }

    void resizeOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        if (currentScale.x - impulse > 0) {
            currentScale.x -= impulse;
            currentPosition.x -= impulse; // Przesuwamy obiekt w dół tylko jeśli skala jest większa od zera
        } else {
            currentScale.x = 0; // Ustawiamy wartość skali na zero, jeżeli nowa wartość byłaby ujemna
        }

        parentTransform->setScale(currentScale);
        parentTransform->setPosition(currentPosition);

        shader->setFloat("scale", currentScale.x);
    }

    void increaseOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        if (currentScale.x + impulse <= 0.166) {
            // Zwiększamy rozmiar paska
            currentScale.x += impulse;
            currentPosition.x += impulse; // Przesuwamy obiekt w górę tylko jeśli skala jest mniejsza od 0.166
        } else {
            currentScale.x = 0.166; // Ustawiamy wartość skali na 0.166, jeżeli nowa wartość byłaby większa
        }

        parentTransform->setScale(currentScale);
        parentTransform->setPosition(currentPosition);

        shader->setFloat("scale", currentScale.x);
    }

    void showScale() {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        std::cout << "Scale: " << currentScale.x << " " << currentScale.y << " " << currentScale.z << std::endl;
    }

};

#endif