#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include "ECS/Component.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "Scene/Transform2.h"
#include "stb_image.h"

class Image: public Component {
public:
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
    unsigned int textureID;
    Shader* shader;

    void awake() override;

    void start() override;

    void update() override ;

    void onDestroy() override;

    ~Image() override = default;

    Transform2* getTransform()
    {

        return parentTransform;
    }

    Image(Shader* shader) : shader(shader) {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    void setTexture(const std::string& texturePath) {
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        stbi_set_flip_vertically_on_load(1); // Dodajemy tę linię

        int width, height, nrChannels;
        unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void renderPlane()
    {
        shader->use();
        glBindTexture(GL_TEXTURE_2D, textureID);
        glm::mat4 model = glm::mat4(1.0f);
        shader->setInt("imageTexture", 0);

        model = glm::translate(model, parentTransform->getLocalPosition());
        //model = glm::rotate(model, glm::radians(parentTransform->getLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z axis
        model = glm::scale(model, parentTransform->getLocalScale());
        shader->setMat4("model", model);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

};

#endif