#include "Script/SceneScript.h"

#include "windows.h"
#include "Debug/Profiler.h"
#include "tracy/TracyOpenGL.hpp"
#include "Text/Text.h"

#include "Engine/Engine.h"
#include "Editor/Editor.h"
#include "Core/AssetManager/AssetManager.h"
#include "ThirdPersonCamera.h"
#include "Physics/ColliderComponent.h"
#include "Physics/CollisionManager.h"
#include "Editor/Gizmos.h"
#include "Input/DebugInput.h"
#include "HUD/Image.h"
#include "HUD/Button.h"
#include "HUD/ResizableImage.h"
#include "Renderer/MaterialAsset.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/SSAO.h"
#include "Generative-System/SongAnalizer.h"
#include "Generative-System/Ball.h"
#include "JoyShockLibrary.h"
#include "Core/Utils/MathUtils.h"
#include "Core/Utils/Ease.h"
#include "Script/menuSceneScript.h"
#include "Script/exampleSceneScript.h"

using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    // Tworzenie zasobów
    EditorLayer::Editor editor;
    SceneManager sm;

    AudioManager& audioManager(AudioManager::getInstance());
    std::shared_ptr<Sound> sound(audioManager.loadSound("res/content/sounds/songs/queen.wav"));

    int connectedControllers = JslConnectDevices();
    PlayerInput playerInput(GLFW_JOYSTICK_1);
    PlayerInput playerInput1(GLFW_JOYSTICK_2);
//    glm::vec2 joystickOffset(0);
//    glm::vec2 joystickOffset2(0);
//    glm::vec2 joystickOffset3(0);
//    glm::vec2 joystickOffset4(0);
    DebugInput debugInput;

    Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");
    Shader imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl");
    Shader imageShaderGreen("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_green.glsl");
    Renderer renderer(&shader);

    double lastTime = 0.0;

    // Inicjalizacja zasobów
    editor.init(&s.camera);
    audioManager.init();
    renderer.init();
    screenShader.use();
    screenShader.setInt("screenTexture", 0);
    AudioManager::getInstance().playSound("res/content/sounds/songs/queen.wav", 1.0f);

    // Tworzenie i ładowanie scen
    //menuSceneScript* menusceneScript = new menuSceneScript(editor, audioManager, sm, playerInput, playerInput1, debugInput, shader, shaderText, shaderPbr, screenShader, imageShader, imageShaderGreen, renderer, lastTime);
    menuSceneScript* menusceneScript = new menuSceneScript(&editor, &audioManager, &sm, &playerInput, &playerInput1, &debugInput, &shader, &shaderText, &shaderPbr, &screenShader, &imageShader, &imageShaderGreen, &renderer, lastTime);
    menusceneScript->awake();
    menusceneScript->start();

    exampleSceneScript* examplesceneScript = new exampleSceneScript(&editor, &audioManager, &sm, &playerInput, &playerInput1, &debugInput, &shader, &shaderText, &shaderPbr, &screenShader, &imageShader, &imageShaderGreen, &renderer, lastTime);
    examplesceneScript->awake();
    examplesceneScript->start();

    // Ustawianie aktualnej sceny na menuSceneScript
    sm.setCurrentScene("KubaScene");

    bool switched = true;
    Scene2* currentScene;
    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();

        // Aktualizacja wspólnych elementów
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

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

    // Wywołanie metody onDestroy dla wszystkich załadowanych scen
    menusceneScript->onDestroy();
    //examplesceneScript->onDestroy();

    // Zakończenie pracy audioManagera
    audioManager.end();

    end();
    return 0;
}
