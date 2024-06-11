//
// Created by Radek on 10.06.2024.
//

#ifndef ENGINE_LIGHTSOURCE_H
#define ENGINE_LIGHTSOURCE_H

#include "ECS/Component.h"
#include "Shader.h"

enum LightType{
    SPOT,
    DIR,
    POINTLIGHT
};

class LightSource : public Component {
public:
    glm::vec3 Color;
    LightType lightType;

    explicit LightSource(LightType lighttype1, const glm::vec3 &color = glm::vec3(1,1,1));

    ~LightSource() override;

    void update() override{};

    virtual void setValuesToShader(Shader&, int);

    glm::vec3 extractEulerAngles(const glm::mat4& transform) {
        glm::vec3 euler;

        // Jeżeli Twoja macierz jest ortogonalna (nie zawiera skalowania):
        glm::mat3 rotationMat = glm::mat3(transform);

        // Obliczenie kątów Eulera z macierzy obrotu
        euler.y = asin(-rotationMat[2][0]);

        if (cos(euler.y) != 0) {
            euler.x = atan2(rotationMat[2][1], rotationMat[2][2]);
            euler.z = atan2(rotationMat[1][0], rotationMat[0][0]);
        } else {
            euler.x = atan2(-rotationMat[1][2], rotationMat[1][1]);
            euler.z = 0;
        }

        return euler;
    }

};
#endif //ENGINE_LIGHTSOURCE_H
