#include "Engine/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"
#include "Script/menuSceneScript.h"
#include "Script/pauseSceneScript.h"
#include "Script/loseSceneScript.h"
#include "Script/winSceneScript.h"
#include "Script/creditsSceneScript.h"
#include "Script/songSceneScript.h"



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

    Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl"),
            collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag"),
            shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl"),
            colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl"),
            shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl"),
            screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag"),
            shaderRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            shaderBarmanRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            shaderDjRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            DrunkShader("res/content/shaders/SSAO/ssao.vert", "res/content/shaders/chromaticAberration.frag"),
            shaderNoneDrink("res/content/shaders/SSAO/ssao.vert", "res/content/shaders/framebuffer.frag"),
            reverseShader("res/content/shaders/SSAO/ssao.vert","res/content/shaders/reverse.frag"),
            imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl"),
            imageShaderGreen("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_green.glsl"),
            shaderRigInstanced("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            shaderRigInstanced2("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/SSAO/ssao_fragment.frag");

    SSAO ssao;

    //Renderer renderer(&shader);
    Renderer renderer(&ssao.shaderGeometryPass);

    //inits
    audioManager.init();
    sm.loadScene("res/content/maps/Kuba.yaml");
    sm.loadScene("res/content/maps/Marcin.yaml");
    sm.loadScene("res/content/maps/Pause.yaml");
    sm.loadScene("res/content/maps/Lose.yaml");
    sm.loadScene("res/content/maps/Win.yaml");
    sm.loadScene("res/content/maps/Credits.yaml");
    sm.loadScene("res/content/maps/Song.yaml");
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
    renderer.init();
    editor.init(&s.camera);
    //inits - end


    //instance
    exampleSceneScript* examplesceneScript = new exampleSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                                    shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                                    shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                                    imageShaderGreen, shaderRigInstanced, shaderDjRig, shaderRigInstanced2);
    //awake
    examplesceneScript->awake();
    //start
    examplesceneScript->start();


    menuSceneScript* menusceneScript = new menuSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                           shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                           shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                           imageShaderGreen, shaderRigInstanced);

    menusceneScript->awake();
    menusceneScript->start();

    pauseSceneScript* pausesceneScript = new pauseSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                           shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                           shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                           imageShaderGreen, shaderRigInstanced);

    pausesceneScript->awake();
    pausesceneScript->start();

    loseSceneScript* losesceneScript = new loseSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                              shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                              shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                              imageShaderGreen, shaderRigInstanced);

    losesceneScript->awake();
    losesceneScript->start();

    winSceneScript* winsceneScript = new winSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                           shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                           shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                           imageShaderGreen, shaderRigInstanced);

    winsceneScript->awake();
    winsceneScript->start();

    creditsSceneScript* creditssceneScript = new creditsSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                        shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                        shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                        imageShaderGreen, shaderRigInstanced);

    creditssceneScript->awake();
    creditssceneScript->start();

//    songSceneScript* songsceneScript = new songSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
//                                                                    shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
//                                                                    shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
//                                                                    imageShaderGreen, shaderRigInstanced);
//
//    songsceneScript->awake();
//    songsceneScript->start();

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
            else if (currentScene->getName() == "PauseScene") {
                // Wywołanie metody update dla exampleSceneScript
                examplesceneScript->onExit();
                pausesceneScript->update();
            }
            else if (currentScene->getName() == "LoseScene") {
                // Wywołanie metody update dla exampleSceneScript
                losesceneScript->update();
            }
            else if (currentScene->getName() == "WinScene") {
                // Wywołanie metody update dla exampleSceneScript
                winsceneScript->update();
            }
            else if (currentScene->getName() == "CreditsScene") {
                // Wywołanie metody update dla exampleSceneScript
                creditssceneScript->update();
            }
//            else if (currentScene->getName() == "SongScene") {
//                // Wywołanie metody update dla exampleSceneScript
//                songsceneScript->update();
//            }
        }

        // Sprawdzanie, czy klawisz spacji został naciśnięty
        if (glfwGetKey(s.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (switched) {
                // Zmiana sceny na exampleSceneScript
                sm.setCurrentScene("MarcinScene");
                switched = false;
            } else {
                // Zmiana sceny na menuSceneScript
                //sm.setCurrentScene("PauseScene");
                sm.setCurrentScene("WinScene");
                switched = true;
            }
        }

        update();
    }
    //onDestroy
    examplesceneScript->onDestroy();
    audioManager.end();


    end();
    return 0;
}