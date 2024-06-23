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
            shaderRigInstanced("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/SSAO/ssao_fragment.frag");

    SSAO ssao;

    //Renderer renderer(&shader);
    Renderer renderer(&ssao.shaderGeometryPass);

    //inits
    audioManager.init();
    sm.loadScene("res/content/maps/Kuba.yaml");
    sm.loadScene("res/content/maps/Marcin.yaml");
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
    renderer.init();
    editor.init(&s.camera);
    //inits - end


    //instance
    exampleSceneScript* examplesceneScript = new exampleSceneScript(editor, cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                                    shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                                    shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                                    imageShaderGreen, shaderRigInstanced, shaderDjRig);
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

        // Sprawdzanie, czy klawisz spacji został naciśnięty
        if (glfwGetKey(s.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (switched) {
                // Zmiana sceny na exampleSceneScript
                sm.setCurrentScene("MarcinScene");
                switched = false;
            } else {
                // Zmiana sceny na menuSceneScript
                sm.setCurrentScene("KubaScene");
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