#ifndef ENGINE_PROGRESSBAR_H
#define ENGINE_PROGRESSBAR_H
#include "Image.h"

class ProgressBar : public Image {
private:
    float currentValue;
    float maxValue;

public:
    ProgressBar(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath, float maxValue)
            : Image(vertexShaderPath, fragmentShaderPath, texturePath), maxValue(maxValue), currentValue(maxValue) {}

    void update(float deltaTime) {
        currentValue -= 2.0f * deltaTime; // zmniejszanie wartości paska postępu

        if (currentValue < 0) {
            currentValue = 0;
        }

        // Obliczanie procentowej wartości paska postępu
        float percentage = currentValue / maxValue;

        // Aktualizacja szerokości paska postępu
        std::vector<float> vertices = {
                300.0f, 100.0f, 0.0f, 0.0f,   // lewy dolny róg
                300.0f, 150.0f, 0.0f, 1.0f,   // lewy górny róg
                300.0f + 300.0f * percentage, 150.0f, 1.0f, 1.0f,   // prawy górny róg
                300.0f + 300.0f * percentage, 100.0f, 1.0f, 0.0f    // prawy dolny róg
        };

        updateVertices(vertices);
    }

    void renderBar() {
        // Użycie shadera
        shader.use();

        // Ustawienie koloru
        float percentage = currentValue / maxValue;
        if (percentage > 0.5f) {
            setColor(glm::vec3(0.0f, 1.0f, 0.0f)); // zielony
        } else if (percentage > 0.2f) {
            setColor(glm::vec3(1.0f, 0.5f, 0.0f)); // pomarańczowy
        } else {
            setColor(glm::vec3(1.0f, 0.0f, 0.0f)); // czerwony
        }

        // Reszta metody render z klasy Image
        Image::render();
    }
};
#endif