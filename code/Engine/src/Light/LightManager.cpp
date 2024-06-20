//
// Created by Radek on 10.06.2024.
//
#include "Light/LightManager.h"
#include "Shader.h"

std::vector<LightSource*> LightManager::Lights;
int LightManager::MaxLights = 30;
int LightManager::SpotLightNum = 0;
int LightManager::PointLightsNum = 0;

void LightManager::AddLight(LightSource* light) {

    if(Lights.size() > MaxLights)
    {
        return;
    }

    switch (light->lightType) {
        case LightType::SPOT:
        {
            if(SpotLightNum > MaxLights)
            {
                LOG_WARNING("Max amount of lights exceeds. Another light sources will be discarded from rendering.");
                return;
            }

            SpotLightNum++;
            break;
        }
        case LightType::POINTLIGHT:
        {
            if(PointLightsNum > MaxLights)
            {
                LOG_WARNING("Max amount of lights exceeds. Another light sources will be discarded from rendering.");
                return;
            }

            PointLightsNum++;
            break;
        }
    }

    Lights.push_back(light);

}

void LightManager::UpdateLightShader(Shader& shader, glm::mat4 view) {

    shader.use();
    for (int i = 0; i < Lights.size(); ++i) {
        Lights[i]->setValuesToShader(shader, Lights[i]->lightType == LightType::SPOT ? SpotLightNum - (SpotLightNum - i) - 1 :
        PointLightsNum - (PointLightsNum - i) - 1, view);
    }

    shader.setInt("spotNum", SpotLightNum);
    shader.setInt("pointNum", PointLightsNum);
}

void LightManager::RemoveLight(LightSource *light) {
    switch (light->lightType) {
        case LightType::SPOT:
        {
            SpotLightNum--;
            break;
        }
        case LightType::POINTLIGHT:
        {
            PointLightsNum--;
            break;
        }
    }
    Lights.erase(std::remove(Lights.begin(), Lights.end(), light), Lights.end());
}

