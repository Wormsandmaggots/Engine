#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

class Image {
private:
    unsigned int VAO, VBO, EBO, texture;
    Shader shader;

public:
    Image(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& texturePath)
            : shader(vertexShaderPath.c_str(), fragmentShaderPath.c_str()) {
        // Inicjalizacja VAO, VBO, EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Definicja struktury danych wierzchołków i indeksów
        float vertices[] = {
                300.0f, 200.0f, 0.0f, 0.0f,   // lewy dolny róg
                300.0f, 500.0f, 0.0f, 1.0f,   // lewy górny róg
                900.0f, 500.0f, 1.0f, 1.0f,   // prawy górny róg
                900.0f, 200.0f, 1.0f, 0.0f    // prawy dolny róg
        };

        unsigned int indices[] = {
                0, 1, 2,   // pierwszy trójkąt
                0, 2, 3    // drugi trójkąt
        };

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        // Załadowanie shaderów
        shader.use();

        // Załadowanie tekstury
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
// Ustawienie parametrów tekstury
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// Wczytanie i generowanie tekstury
        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            if (nrChannels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else if (nrChannels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else
                std::cout << "Unsupported number of channels in texture" << std::endl;
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void render() {
        // Użycie shadera
        shader.use();

        // Ustawienie macierzy modelu, widoku i projekcji
        glm::mat4 model = glm::mat4(1.0f); // macierz jednostkowa, nie przekształca niczego
        glm::mat4 view = glm::mat4(1.0f); // macierz jednostkowa, nie przekształca niczego
        glm::mat4 projection = glm::ortho(0.0f, (float)1920, 0.0f, (float)1080); // macierz ortograficzna

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // Wiązanie i aktywacja tekstury
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Wiązanie VAO
        glBindVertexArray(VAO);

        // Rysowanie elementów
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    ~Image() {
        // Usuwanie VAO, VBO, EBO i tekstury
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteTextures(1, &texture);
    }
};


#endif //ENGINE_IMAGE_H