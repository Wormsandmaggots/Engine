//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_SCENEMANAGER_H
#define ENGINE_SCENEMANAGER_H


#include "Scene2.h"
#include "Scene/Loader/SceneLoader.h"

class SceneManager {
public:
    SceneManager() = default;
    virtual ~SceneManager() = default;

    void loadScene(const std::string&);
    void unloadScene(const std::string&);
    void updateLoadedScenes();

private:
    std::vector<Scene2*> loadedScenes;
    SceneLoader sceneLoader;
};


#endif //ENGINE_SCENEMANAGER_H
