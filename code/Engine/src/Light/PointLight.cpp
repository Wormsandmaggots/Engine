//
// Created by Radek on 11.06.2024.
//

#include "Light/PointLight.h"
#include "Scene/Transform2.h"

PointLight::PointLight(float radius, float linear, float quadratic, const glm::vec3 &color) :
Radius(radius),
Linear(linear),
Quadratic(quadratic),
LightSource(LightType::POINTLIGHT, color){}

void PointLight::setValuesToShader(Shader &shader, int i) {
    std::string iterator = std::to_string(i);

    shader.setVec3("pointLights[" + iterator + "].position", glm::vec3(parentTransform->getWorldMatrix()[3]));
    shader.setVec3("pointLights[" + iterator + "].color", Color);
    shader.setFloat("pointLights[" + iterator + "].linear", Linear);
    shader.setFloat("pointLights[" + iterator + "].quadratic", Quadratic);
    shader.setFloat("pointLights[" + iterator + "].radius", Radius);
}


