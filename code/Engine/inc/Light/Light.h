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
        lightShader.setFloat("material.shininess", 30.0f);
        //kierunkowe
        lightShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3);
        lightShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
        lightShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        lightShader.setVec3("dirLight.specular", 0.6f, 0.6f, 0.6f);
/*for maximizing spotlight
        lightShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
        lightShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
        lightShader.setVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);
*/
        //punktowe
        lightShader.setVec3("pointLights[0].position", 100,100,100);
        lightShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightShader.setFloat("pointLights[0].constant", 1.0f);
        lightShader.setFloat("pointLights[0].linear", 0.09f);
        lightShader.setFloat("pointLights[0].quadratic", 0.032f);

        lightShader.setVec3("pointLights[1].position", 0,0,0);
        lightShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightShader.setFloat("pointLights[1].constant", 1.0f);
        lightShader.setFloat("pointLights[1].linear", 0.09f);
        lightShader.setFloat("pointLights[1].quadratic", 0.032f);

        //spotlight
        lightShader.setVec3("spotLight.position", 0.0f,11.0f,48.0f);
        lightShader.setVec3("spotLight.direction", -7.0f,-2.0f,1.0f);
        lightShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightShader.setFloat("spotLight.constant", 1.0f);
        lightShader.setFloat("spotLight.linear", 0.09f);
        lightShader.setFloat("spotLight.quadratic", 0.032f);
        lightShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
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