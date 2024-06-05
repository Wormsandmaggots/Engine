#ifndef ENGINE_IMAGE_H
#define ENGINE_IMAGE_H

#include "ECS/Component.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "Scene/Transform2.h"

class Image: public Component {
public:
    unsigned int quadVAO = 0;
    unsigned int quadVBO;
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
/*
    void renderPlane()
    {
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, parentTransform->getLocalPosition());
        model = glm::scale(model, parentTransform->getLocalScale());
        shader->setMat4("model", model);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
*/

    void renderPlane()
    {
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, parentTransform->getLocalPosition());
        model = glm::rotate(model, glm::radians(parentTransform->getLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z axis
        model = glm::scale(model, parentTransform->getLocalScale());
        shader->setMat4("model", model);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

/*
    void renderPlane()
    {
        shader->use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, parentTransform->getLocalPosition());
        model = glm::translate(model, -parentTransform->getPivotPoint()); // Przesunięcie obiektu tak, aby punkt obracania był w środku
        model = glm::scale(model, glm::vec3(1.0f / parentTransform->getLocalScale().x, 1.0f / parentTransform->getLocalScale().y, 1.0f)); // Przekształcenie do przestrzeni świata
        model = glm::rotate(model, glm::radians(parentTransform->getLocalRotation().z), glm::vec3(0.0f, 0.0f, 1.0f)); // Z axis
        model = glm::translate(model, parentTransform->getPivotPoint()); // Przesunięcie obiektu z powrotem
        model = glm::scale(model, parentTransform->getLocalScale()); // Przekształcenie z powrotem do przestrzeni ekranu
        shader->setMat4("model", model);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
*/
};

#endif //ENGINE_IMAGE_H