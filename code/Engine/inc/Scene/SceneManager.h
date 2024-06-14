//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_SCENEMANAGER_H
#define ENGINE_SCENEMANAGER_H

#include "Scene2.h"
#include "Scene/Loader/SceneLoader.h"

namespace SceneManagement {

    class SceneManager {
    public:
        static SceneManager* Instance;
        Scene2* currentScene = nullptr;
        SceneManager();

        virtual ~SceneManager() = default;

        void loadScene(const std::string &);
        void unloadScene(const std::string &);
        void unloadScene(Scene2*);
        void updateLoadedScenes();

        const std::vector<Scene2*>& getLoadedScenes() const;

        void setCurrentScene(const std::string &sceneName) {
            auto foundedScene = std::find_if(loadedScenes.begin(), loadedScenes.end(),
                                             [&sceneName](const Scene2* scene) { return scene->getName() == sceneName; });

            if (foundedScene != loadedScenes.end()) {
                currentScene = *foundedScene;
            } else {
                LOG_ERROR("Scene '" + sceneName + "' has not been loaded before.\n");
            }
        }

        Scene2* getCurrentScene() const {
            return currentScene;
        }

        void updateEntities() {
            if (currentScene) {
                for (auto& entity : currentScene->getSceneEntities()) {
                    entity->update();
                }
            }
        }

    private:
        std::vector<Scene2 *> loadedScenes;
        SceneLoader sceneLoader;
    };
}

#endif //ENGINE_SCENEMANAGER_H
