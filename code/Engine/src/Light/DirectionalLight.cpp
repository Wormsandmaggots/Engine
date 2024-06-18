//
// Created by Radek on 11.06.2024.
//

#include "Light/DirectionalLight.h"
#include "Scene/Transform2.h"

DirectionalLight::DirectionalLight(const glm::vec3 &color) :  LightSource(LightType::DIR, color){
}

void DirectionalLight::setValuesToShader(Shader &shader, int, glm::mat4) {
    shader.setVec3("dirLight.direction", glm::normalize(parentTransform->getLocalRotation()));
    shader.setFloat("dirLight.intensity", 1.f);
    shader.setVec3("dirLight.colors.diffuse", parentTransform->getLocalScale());
    shader.setVec3("dirLight.colors.ambient", parentTransform->getLocalScale());
    shader.setVec3("dirLight.colors.specular", parentTransform->getLocalScale());
}
