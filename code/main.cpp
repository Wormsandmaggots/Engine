#include "Engine/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"



using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    EditorLayer::Editor editor;
    CollisionManager cm;
    SceneManager sm;
    AudioManager& audioManager(AudioManager::getInstance());
    PlayerInput playerInput(GLFW_JOYSTICK_1);
    PlayerInput playerInput1(GLFW_JOYSTICK_2);
    DebugInput debugInput;
    Shader shader("path_to_vertex_shader", "path_to_fragment_shader");

    //instance
    exampleSceneScript* examplesceneScript = new exampleSceneScript(editor, cm, sm, audioManager, playerInput, playerInput1, debugInput, shader);
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