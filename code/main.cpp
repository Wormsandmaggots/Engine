#include "Engine/inc/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"


using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    //instance
    exampleSceneScript* examplesceneScript = new exampleSceneScript();
    //awake
    examplesceneScript->awake();
    //start
    examplesceneScript->start();

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        //update
        examplesceneScript->update();
        update();
    }
    //onDestroy
    examplesceneScript->onDestroy();


    end();
    return 0;
}