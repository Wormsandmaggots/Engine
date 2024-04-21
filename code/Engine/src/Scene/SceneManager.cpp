//
// Created by Radek on 25.03.2024.
//

#include "Engine/inc/Scene/SceneManager.h"
#include "Debug/Logger.h"

using namespace SceneManagement;

SceneManager* SceneManager::Instance = nullptr;

void SceneManager::loadScene(const string &path) {
    Entity::ResetCounter();
    loadedScenes.push_back(sceneLoader.loadScene(path));
}

void SceneManager::unloadScene(const std::string &sceneName) {
    auto foundedScene = std::find_if(loadedScenes.begin(), loadedScenes.end(),
                           [&sceneName](const Scene* scene) { return scene->getName() == sceneName; });

    if (foundedScene != loadedScenes.end()) {
        delete *foundedScene;
        loadedScenes.erase(std::remove(loadedScenes.begin(), loadedScenes.end(), *foundedScene), loadedScenes.end());
        //loadedScenes.erase(foundedScene);
        LOG_INFO("Scene '" + sceneName + "' has been correctly unloaded.\n");
    } else {
        LOG_ERROR("Scene '" + sceneName + "' has not been loaded before.\n");
    }
}

void SceneManager::updateLoadedScenes() {
    for (Scene *scene : loadedScenes) {
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

const std::vector<Scene *> &SceneManager::getLoadedScenes() const {
    return loadedScenes;
}

void SceneManager::unloadScene(Scene* toUnload) {
    delete toUnload;
    loadedScenes.erase(std::remove(loadedScenes.begin(), loadedScenes.end(), toUnload), loadedScenes.end());
}
