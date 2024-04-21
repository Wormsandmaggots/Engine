//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_SCENELOADER_H
#define ENGINE_SCENELOADER_H

#include "YamlReader.h"
#include "Scene/Scene.h"

class SceneLoader {
public:
    SceneLoader() = default;
    virtual ~SceneLoader() = default;

    [[nodiscard]] Scene* loadScene(std::string path);

private:
    YamlReader reader;
};


#endif //ENGINE_SCENELOADER_H
