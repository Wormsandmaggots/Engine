#ifndef ENGINE_PROGRESSBAR_H
#define ENGINE_PROGRESSBAR_H
#include "Image.h"
#include "Core/Time.h"

class ProgressBar : public Image {
private:
    float currentValue;
    float maxValue;

public:
    ProgressBar(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath, float maxValue)
            : Image(vertexShaderPath, fragmentShaderPath, texturePath), maxValue(maxValue), currentValue(maxValue) {}

    void update() override {
        currentValue -= 2.0f * deltaTime;

        if (currentValue < 0) {
            currentValue = 0;
        }

        float percentage = 1;

        std::vector<float> vertices = {
                300.0f, 100.0f, 0.0f, 0.0f,
                300.0f, 150.0f, 0.0f, 1.0f,
                300.0f + 300.0f * percentage, 150.0f, 1.0f, 1.0f,
                300.0f + 300.0f * percentage, 100.0f, 1.0f, 0.0f
        };

        updateVertices(vertices);

        renderBar();
    }

    void renderBar() {
        shader.use();

        float percentage = currentValue / maxValue;
        if (percentage > 0.5f) {
            setColor(glm::vec3(0.0f, 1.0f, 0.0f));
        } else if (percentage > 0.2f) {
            setColor(glm::vec3(1.0f, 0.5f, 0.0f));
        } else {
            setColor(glm::vec3(1.0f, 0.0f, 0.0f));
        }

        Image::render();
    }
};
#endif