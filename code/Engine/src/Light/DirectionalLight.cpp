//
// Created by Radek on 11.06.2024.
//

#include "Light/DirectionalLight.h"
#include "Scene/Transform2.h"

DirectionalLight::DirectionalLight(const glm::vec3 &color) :  LightSource(LightType::DIR, color){
}

void DirectionalLight::setValuesToShader(Shader &shader, int) {
    shader.setVec3("dirLight.direction", glm::normalize(glm::vec3(-0.2f, -1.0f, -0.3f)));
    shader.setVec3("dirLight.color", Color);
}
