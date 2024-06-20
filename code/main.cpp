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
    //instance
    menuSceneScript* menusceneScript = new menuSceneScript();
    //awake
    menusceneScript->awake();
    //start
    menusceneScript->start();

    //instance
    //exampleSceneScript* examplesceneScript = new exampleSceneScript();
    //awake
    //examplesceneScript->awake();
    //start
    //examplesceneScript->start();

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        //update
        //examplesceneScript->update();
        menusceneScript->update();

        update();
    }
    //onDestroy
    //examplesceneScript->onDestroy();
    menusceneScript->onDestroy();


    end();
    return 0;
}