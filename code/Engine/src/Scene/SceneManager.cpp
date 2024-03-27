//
// Created by Radek on 25.03.2024.
//

#include "Engine/inc/Scene/SceneManager.h"
#include "Debug/Logger.h"

using namespace SceneManagement;

SceneManager* SceneManager::Instance = nullptr;

void SceneManager::loadScene(const string &path) {
    loadedScenes.push_back(sceneLoader.loadScene(path));
}

void SceneManager::unloadScene(const std::string &sceneName) {
    auto foundedScene = std::find_if(loadedScenes.begin(), loadedScenes.end(),
                           [&sceneName](const Scene2* scene) { return scene->getName() == sceneName; });

    if (foundedScene != loadedScenes.end()) {
        delete *foundedScene;
        loadedScenes.erase(foundedScene);
        LOG_INFO("Scene '" + sceneName + "' has been correctly unloaded.\n");
    } else {
        LOG_ERROR("Scene '" + sceneName + "' has not been loaded before.\n");
    }
}

void SceneManager::updateLoadedScenes() {
    for (Scene2 *scene : loadedScenes) {
        scene->update();
    }
}

SceneManager::SceneManager() {
    if(SceneManager::Instance == nullptr)
    {
        SceneManager::Instance = this;
    }
    else
    {
        delete this;
    }
}

const std::vector<Scene2 *> &SceneManager::getLoadedScenes() const {
    return loadedScenes;
}
