//
// Created by Radek on 11.06.2024.
//

#include "Light/SpotLight.h"
#include "Scene/Transform2.h"

SpotLight::SpotLight(float cutoff, float outercutoff, glm::vec3 color) :
Cutoff(cutoff),
Outercutoff(outercutoff),
LightSource(LightType::SPOT, color){
}

void SpotLight::setValuesToShader(Shader &shader, int i, glm::mat4 view) {
    std::string iterator = std::to_string(i);

    shader.setVec3("spotLights[" + iterator + "].position", glm::vec4(parentTransform->getWorldMatrix()[3]));
    shader.setVec3("spotLights[" + iterator + "].direction", extractEulerAngles(parentTransform->getWorldMatrix()));
    shader.setVec3("spotLights[" + iterator + "].colors.diffuse", Color);
    shader.setFloat("spotLights[" + iterator + "].cutoff", Cutoff);
    shader.setFloat("spotLights[" + iterator + "].outercutoff", Outercutoff);
}


