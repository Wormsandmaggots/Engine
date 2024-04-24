#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "Shader.h"

class Light : public Component {
public:
    Shader& lightShader;

    Light(Shader& shader) : lightShader(shader) {}

    virtual ~Light() = default;

    void awake() override {
        // Implementacja metody awake
    }

    void start() override {
        // Implementacja metody start
    }

    void update() override {
        lightShader.use();
        lightShader.setFloat("material.shininess", 1.0f);
        lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3);
        lightShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        lightShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        lightShader.setVec3("dirLight.specular", 0.6f, 0.6f, 0.6f);
    }

    void onDestroy() override {
        // Implementacja metody onDestroy
    }
};
#endif

//TODO: implement this in future version
/*
        //Obliczanie znormalizowanegej macierzy modelu player
        glm::mat4 model = sm.getLoadedScenes()[0]->getSceneEntities()[2]->getComponent<Model>()->getModelMatrixInWorldSpace();
        // Obliczanie macierzy normalnej
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        // Przekazanie macierzy normalnej do shadera
        lightShader.setMat3("normalMatrix", normalMatrix);
*/