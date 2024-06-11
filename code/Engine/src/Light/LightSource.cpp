//
// Created by Radek on 10.06.2024.
//

#include "Light/LightSource.h"
#include "Light/LightManager.h"

LightSource::LightSource(LightType lighttype1, const glm::vec3 &color) : lightType(lighttype1), Color(color) {
    LightManager::AddLight(this);
}

void LightSource::setValuesToShader(Shader &, int) {

}

LightSource::~LightSource() {
    LightManager::RemoveLight(this);
}
