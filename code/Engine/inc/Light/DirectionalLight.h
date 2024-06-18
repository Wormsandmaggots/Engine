//
// Created by Radek on 11.06.2024.
//

#ifndef ENGINE_DIRECTIONALLIGHT_H
#define ENGINE_DIRECTIONALLIGHT_H
#include "LightSource.h"


class DirectionalLight : public LightSource{
public:

    explicit DirectionalLight(const glm::vec3 &color = glm::vec3(1,1,1));

    void setValuesToShader(Shader &, int, glm::mat4) override;
};


#endif //ENGINE_DIRECTIONALLIGHT_H
