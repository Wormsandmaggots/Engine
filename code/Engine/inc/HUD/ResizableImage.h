#ifndef ENGINE_RESIZABLEIMAGE_H
#define ENGINE_RESIZABLEIMAGE_H

#include "HUD/Image.h"

class ResizableImage : public Image {
public:
    ResizableImage(Shader* shader) : Image(shader) {}

    void resizeOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        if (currentScale.y - impulse > 0) {
            currentScale.y -= impulse;
            parentTransform->setScale(currentScale);

            // Przesunięcie obiektu w dół
            currentPosition.y -= impulse;
            parentTransform->setPosition(currentPosition);
        }
    }

    void increaseOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        // Zwiększamy rozmiar paska
        currentScale.y += impulse;
        parentTransform->setScale(currentScale);

        // Przesuwamy obiekt w górę
        currentPosition.y += impulse;
        parentTransform->setPosition(currentPosition);
    }

};

#endif