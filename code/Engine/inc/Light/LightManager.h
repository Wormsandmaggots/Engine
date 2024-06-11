//
// Created by Radek on 10.06.2024.
//

#ifndef ENGINE_LIGHTMANAGER_H
#define ENGINE_LIGHTMANAGER_H

#include <vector>
#include "LightSource.h"

class Shader;

class LightManager{
public:
    static std::vector<LightSource*> Lights;
    static int MaxLights;
    static int SpotLightNum;
    static int PointLightsNum;

    static void AddLight(LightSource*);
    static void RemoveLight(LightSource*);
    static void UpdateLightShader(Shader&);
};

#endif //ENGINE_LIGHTMANAGER_H
