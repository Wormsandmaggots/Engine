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

    PointLight(float radius = 1.f,
               float linear = 0.09f,
               float quadratic = 0.032f,
               const glm::vec3 &color = glm::vec3(1,1,1));

    void setValuesToShader(Shader &, int) override;
};


#endif //ENGINE_POINTLIGHT_H
