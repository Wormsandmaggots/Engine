//
// Created by Radek on 11.06.2024.
//

#ifndef ENGINE_SPOTLIGHT_H
#define ENGINE_SPOTLIGHT_H
#include "LightSource.h"

class SpotLight : public LightSource{
public:
    float Cutoff;
    float Outercutoff;

    SpotLight(float cutoff = glm::cos(glm::radians(12.5f)),
              float outercutoff = glm::cos(glm::radians(17.5f)),
              glm::vec3 color = glm::vec3(1,1,1));

    void setValuesToShader(Shader &, int) override;
};


#endif //ENGINE_SPOTLIGHT_H
