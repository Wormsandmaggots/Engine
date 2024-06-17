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

    //instance
    menuSceneScript* menusceneScript = new menuSceneScript();
    exampleSceneScript* examplesceneScript = new exampleSceneScript();
    //awake
    menusceneScript->awake();
    examplesceneScript->awake();
    //start
    menusceneScript->start();
    examplesceneScript->start();

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        //update
        menusceneScript->update();
        examplesceneScript->update();
        
        update();
    }
    //onDestroy
    menusceneScript->onDestroy();
    examplesceneScript->onDestroy();


    end();
    return 0;
}