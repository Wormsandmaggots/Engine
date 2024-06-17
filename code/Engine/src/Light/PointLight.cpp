//
// Created by Radek on 11.06.2024.
//

#include "Light/PointLight.h"
#include "Scene/Transform2.h"
const float constant = 1.0f; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
PointLight::PointLight(float linear, float quadratic, const glm::vec3 &color) :
Linear(linear),
Quadratic(quadratic),
LightSource(LightType::POINTLIGHT, color){
    const float maxBrightness = std::fmaxf(std::fmaxf(color.r, color.g), color.b);
    Radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
}

void PointLight::setValuesToShader(Shader &shader, int i, glm::mat4 view) {
    std::string iterator = "pointLights[" + std::to_string(i) + "].";

    shader.setVec3(iterator + "position", glm::vec3((view * glm::vec4(parentTransform->getWorldMatrix()[3]))[3]));
    shader.setVec3(iterator + "colors.ambient", parentTransform->getLocalScale());
    shader.setVec3(iterator + "colors.diffuse", parentTransform->getLocalScale());
    shader.setVec3(iterator + "colors.specular", parentTransform->getLocalScale());
    shader.setFloat(iterator + "linear", Linear);
    shader.setFloat(iterator + "quadratic", Quadratic);
    shader.setFloat(iterator + "radius", Radius);
}


