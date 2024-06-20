#include "Engine/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"
#include "Script/menuSceneScript.h"
#include "Engine/inc/Scene/SceneManager.h"

using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    // Tworzenie i ładowanie scen
    menuSceneScript* menusceneScript = new menuSceneScript();
    menusceneScript->awake();
    menusceneScript->start();

    //sceneManager->loadScene("menuScene"); // Załóżmy, że "menuScene" to nazwa sceny menuSceneScript

    exampleSceneScript* examplesceneScript = new exampleSceneScript();
    examplesceneScript->awake();
    examplesceneScript->start();
    //sceneManager->loadScene("exampleScene"); // Załóżmy, że "exampleScene" to nazwa sceny exampleSceneScript

    // Tworzenie instancji SceneManager
    SceneManager* sceneManager = SceneManager::Instance;
    // Ustawianie aktualnej sceny na menuSceneScript
    sceneManager->setCurrentScene("KubaScene");

    bool isMenuScene = true;

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();

        while (!glfwWindowShouldClose(s.window))
        {
            imgui_begin();

            // Pobieranie aktualnej sceny
            Scene2* currentScene = sceneManager->getCurrentScene();
            if (currentScene != nullptr) {
                // Sprawdzanie, która scena jest aktualnie aktywna
                if (currentScene->getName() == "KubaScene") {
                    // Wywołanie metody update dla menuSceneScript
                    menusceneScript->update();
                } else if (currentScene->getName() == "MarcinScene") {
                    // Wywołanie metody update dla exampleSceneScript
                    examplesceneScript->update();
                }
            }

            // Sprawdzanie, czy klawisz spacji został naciśnięty
            if (glfwGetKey(s.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                if (isMenuScene) {
                    // Zmiana sceny na exampleSceneScript
                    sceneManager->setCurrentScene("MarcinScene");
                    isMenuScene = false;
                } else {
                    // Zmiana sceny na menuSceneScript
                    sceneManager->setCurrentScene("KubaScene");
                    isMenuScene = true;
                }
            }

            update();
        }

        update();
    }

    // Wywołanie metody onDestroy dla wszystkich załadowanych scen
    menusceneScript->onDestroy();
    examplesceneScript->onDestroy();

    end();
    return 0;
}