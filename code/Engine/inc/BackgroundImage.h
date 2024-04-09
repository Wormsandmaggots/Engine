#ifndef ENGINE_BACKGROUNDIMAGE_H
#define ENGINE_BACKGROUNDIMAGE_H

#include "Image.h"
class BackgroundImage : public Image {
private:
    float textureOffset = 0.0f; // przesunięcie tekstury
public:
    BackgroundImage(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath)
            : Image(vertexShaderPath, fragmentShaderPath, texturePath) {
        // Definicja struktury danych wierzchołków i indeksów dla tła
        std::vector<float> vertices = {
                300.0f, 100.0f, 0.0f, 0.0f,   // lewy dolny róg
                300.0f, 350.0f, 0.0f, 1.0f,   // lewy górny róg
                650.0f, 350.0f, 1.0f, 1.0f,   // prawy górny róg
                650.0f, 100.0f, 1.0f, 0.0f    // prawy dolny róg
        };

        updateVertices(vertices);
    }

    void update(float deltaTime) {
        textureOffset += deltaTime; // aktualizacja przesunięcia tekstury
        if (textureOffset > 1.0f) {
            textureOffset -= 1.0f; // resetowanie przesunięcia, gdy przekroczy 1.0
        }

        // Aktualizacja koordynat tekstury
        std::vector<float> vertices = {
                300.0f, 100.0f, 0.0f + textureOffset, 0.0f,   // lewy dolny róg
                300.0f, 350.0f, 0.0f + textureOffset, 1.0f,   // lewy górny róg
                650.0f, 350.0f, 1.0f + textureOffset, 1.0f,   // prawy górny róg
                650.0f, 100.0f, 1.0f + textureOffset, 0.0f    // prawy dolny róg
        };

        updateVertices(vertices);
    }
};

#endif