#ifndef ENGINE_RESIZABLEIMAGE_H
#define ENGINE_RESIZABLEIMAGE_H

#include "HUD/Image.h"

class ResizableImage : public Image {
public:
    ResizableImage(Shader* shader) : Image(shader) {
        shader->use();
        shader->setFloat("scale", 0.2f);
    }

    void resizeOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        if (currentScale.x - impulse > 0) {
            currentScale.x -= impulse;
            parentTransform->setScale(currentScale);

            // Przesunięcie obiektu w dół
            currentPosition.x -= impulse;
            parentTransform->setPosition(currentPosition);
        }

        shader->setFloat("scale", currentScale.x);
    }

    void increaseOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        if (currentScale.x + impulse < 0.3) {
            // Zwiększamy rozmiar paska
            currentScale.x += impulse;
            parentTransform->setScale(currentScale);

            // Przesuwamy obiekt w górę
            currentPosition.x += impulse;
            parentTransform->setPosition(currentPosition);
        }

        shader->setFloat("scale", currentScale.x);
    }

    void resetScaleAndPosition() {
        // Ustawiamy skalę na pierwotną wartość
        glm::vec3 originalScale(0.2f, 0.041f, 0.0f);
        parentTransform->setScale(originalScale);
        shader->setFloat("scale", 0.166f);

        // Ustawiamy pozycję na pierwotną wartość
        glm::vec3 originalPosition(-0.699f, -0.699f, 0.0f);
        parentTransform->setPosition(originalPosition);
    }

    void showScale() {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        std::cout << "Scale: " << currentScale.x << " " << currentScale.x << " " << currentScale.z << std::endl;
    }

};

#endif