#ifndef ENGINE_BACKGROUNDIMAGE_H
#define ENGINE_BACKGROUNDIMAGE_H

#include "Image.h"
class BackgroundImage : public Image {
private:
    float textureOffset = 0.0f;
public:
    BackgroundImage(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath)
            : Image(vertexShaderPath, fragmentShaderPath, texturePath) {
        std::vector<float> vertices = {
                300.0f, 100.0f, 0.0f, 0.0f,
                300.0f, 350.0f, 0.0f, 1.0f,
                650.0f, 350.0f, 1.0f, 1.0f,
                650.0f, 100.0f, 1.0f, 0.0f
        };

        updateVertices(vertices);
    }

    void update(float deltaTime) {
        textureOffset += deltaTime;
        if (textureOffset > 1.0f) {
            textureOffset -= 1.0f;
        }

        std::vector<float> vertices = {
                300.0f, 100.0f, 0.0f + textureOffset, 0.0f,
                300.0f, 350.0f, 0.0f + textureOffset, 1.0f,
                650.0f, 350.0f, 1.0f + textureOffset, 1.0f,
                650.0f, 100.0f, 1.0f + textureOffset, 0.0f
        };

        updateVertices(vertices);
    }
};

#endif