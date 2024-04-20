#ifndef ENGINE_LIGHT_H
#define ENGINE_LIGHT_H

#include "Shader.h"

class Light {
public:
    Light(Shader* shader) : shader(shader) {}

    void setLightColors(const glm::vec3& objectColor, const glm::vec3& lightColor) {
        shader->use();
        shader->setVec3("objectColor", objectColor);
        shader->setVec3("lightColor", lightColor);
    }

private:
    Shader* shader;
};
#endif //ENGINE_LIGHT_H
