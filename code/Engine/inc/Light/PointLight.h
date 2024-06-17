//
// Created by Radek on 11.06.2024.
//

#ifndef ENGINE_POINTLIGHT_H
#define ENGINE_POINTLIGHT_H

#include "LightSource.h"

class PointLight : public LightSource{
public:
    float Linear;
    float Quadratic;
    float Radius;

    PointLight(float linear = 0.7f,
               float quadratic = 1.8f,
               const glm::vec3 &color = glm::vec3(1,1,1));

    void setValuesToShader(Shader &, int, glm::mat4) override;
};


#endif //ENGINE_POINTLIGHT_H
