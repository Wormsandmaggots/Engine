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
        SceneManager();

        virtual ~SceneManager() = default;

        void loadScene(const std::string &);
        void unloadScene(const std::string &);
        void unloadScene(Scene2*);
        void updateLoadedScenes();

        const std::vector<Scene2*>& getLoadedScenes() const;

    private:
        std::vector<Scene2 *> loadedScenes;
        SceneLoader sceneLoader;
    };
}

#endif //ENGINE_SCENEMANAGER_H
