#include "Engine/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"
#include "Script/menuSceneScript.h"



using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    SceneScript::mainInit();


    //instance
    exampleSceneScript* examplesceneScript = new exampleSceneScript();
    //awake
    examplesceneScript->awake();
    //start
    examplesceneScript->start();


    menuSceneScript* menusceneScript = new menuSceneScript();

    menusceneScript->awake();
    menusceneScript->start();

// Ustawianie aktualnej sceny na menuSceneScript
    sm.setCurrentScene("KubaScene");

    bool switched = true;
    Scene2* currentScene;

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        //update
        //examplesceneScript->update();
        //menusceneScript->update();

        // Pobieranie aktualnej sceny
        currentScene = sm.getCurrentScene();
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

//        // Sprawdzanie, czy klawisz spacji został naciśnięty
//        if (glfwGetKey(s.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
//            if (switched) {
//                // Zmiana sceny na exampleSceneScript
//                sm.setCurrentScene("MarcinScene");
//                switched = false;
//            } else {
//                // Zmiana sceny na menuSceneScript
//                sm.setCurrentScene("KubaScene");
//                switched = true;
//            }
//        }

        update();
    }
    //onDestroy
    examplesceneScript->onDestroy();
    audioManager.end();


    end();
    return 0;
}