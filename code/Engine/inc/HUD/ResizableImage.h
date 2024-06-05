#ifndef ENGINE_RESIZABLEIMAGE_H
#define ENGINE_RESIZABLEIMAGE_H

#include "HUD/Image.h"

class ResizableImage : public Image {
public:
    ResizableImage(Shader* shader) : Image(shader) {}

    void resizeOnImpulse(float impulse) {
        glm::vec3 currentScale = parentTransform->getLocalScale();
        glm::vec3 currentPosition = parentTransform->getLocalPosition();

        // Skalowanie obiektu
        currentScale.y += impulse;
        parentTransform->setScale(currentScale);

        // Przesunięcie obiektu w górę
        currentPosition.y += impulse;
        parentTransform->setPosition(currentPosition);
    }
};

#endif