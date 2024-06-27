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
#include "Script/calibrationSceneScript.h"
#include <thread>

using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    bool *isLoading = new bool(true);

    Shader sh = Shader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");


    PlayerInput playerInput(GLFW_JOYSTICK_1);
    PlayerInput playerInput1(GLFW_JOYSTICK_2);
    DebugInput debugInput;

    CollisionManager cm;
    SceneManager sm;
    AudioManager& audioManager(AudioManager::getInstance());



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
    sm.loadScene("res/content/maps/Calibration.yaml");
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
    renderer.init();

    //Ustawianie aktualnej sceny na menuSceneScript
    sm.setCurrentScene("KubaScene");

    //CREATING WINDOW
    GLFWwindow* window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "Loading...", NULL, s.window);
    if (window == nullptr)
    {
        LOG_ERROR("Failed to Create GLFW window");
        glfwTerminate();
        exit(-1);
    }

    LOG_INFO("GLFW window created");
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //how often buffers will swap

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD in worker thread");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(s.window);
    std::thread worker([](bool *load, GLFWwindow* window, Shader* sh) {

        glfwMakeContextCurrent(window);

        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                // positions   // texCoords
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                1.0f, -1.0f,  1.0f, 0.0f,

                -1.0f,  1.0f,  0.0f, 1.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
                1.0f,  1.0f,  1.0f, 1.0f
        };

        unsigned int quadVAO, quadVBO;

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        //TODO::Kuba zmień teksturę
        Texture loadTexture = Texture("res/content/loadingscreen2.jpg", "load");


        sh->use();
        sh->setInt("screenTexture", 0);


        while(*load == true)
        {
            glClearColor(0.8, 0.8, 0.8, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            sh->use();
            glActiveTexture(GL_TEXTURE0);
            loadTexture.bind();
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwMakeContextCurrent(window);
            glfwPollEvents();
        }

    }, isLoading, window, &sh);

    worker.detach();

    glfwHideWindow(s.window);


    //inits - end


    //instance
    exampleSceneScript* examplesceneScript = new exampleSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                                    shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                                    shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                                    imageShaderGreen, shaderRigInstanced, shaderDjRig, shaderRigInstanced2);
    //awake
    examplesceneScript->awake();
    //start
    examplesceneScript->start();


    menuSceneScript* menusceneScript = new menuSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                           shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                           shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                           imageShaderGreen, shaderRigInstanced);

    menusceneScript->awake();
    menusceneScript->start();

    pauseSceneScript* pausesceneScript = new pauseSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                           shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                           shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                           imageShaderGreen, shaderRigInstanced);

    pausesceneScript->awake();
    pausesceneScript->start();

    loseSceneScript* losesceneScript = new loseSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                              shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                              shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                              imageShaderGreen, shaderRigInstanced);

    losesceneScript->awake();
    losesceneScript->start();

    winSceneScript* winsceneScript = new winSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                           shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                           shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                           imageShaderGreen, shaderRigInstanced);

    winsceneScript->awake();
    winsceneScript->start();

    creditsSceneScript* creditssceneScript = new creditsSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                        shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                        shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                        imageShaderGreen, shaderRigInstanced);

    creditssceneScript->awake();
    creditssceneScript->start();

    songSceneScript* songsceneScript = new songSceneScript(cm, sm, ssao, renderer, audioManager, playerInput, playerInput1, debugInput,
                                                                    shader, collisionTestShader, shaderText, colorShader, shaderPbr, screenShader,
                                                                    shaderRig, shaderBarmanRig, DrunkShader, shaderNoneDrink, reverseShader, imageShader,
                                                                    imageShaderGreen, shaderRigInstanced);

    songsceneScript->awake();
    songsceneScript->start();

    calibrationSceneScript* calibrationsceneScript = new calibrationSceneScript(cm, sm, ssao, renderer, /*AudioManager &audioManager, */playerInput,
                                                                                playerInput1, debugInput, shader, collisionTestShader, shaderText,
                                                                                colorShader, shaderPbr, screenShader, shaderRig,/* Shader &shaderBarmanRig, Shader &DrunkShader,*/
            /*Shader &shaderNoneDrink, *//*Shader &reverseShader,*/imageShader, imageShaderGreen /*Shader &shaderRigInstanced, Shader &shaderDjRig,Shader &shaderRigInstanced2*/);
    calibrationsceneScript->awake();
    calibrationsceneScript->start();

// Ustawianie aktualnej sceny na menuSceneScript
    sm.setCurrentScene("KubaScene");

    bool switched = true;
    Scene2* currentScene;

    *isLoading = false;

    glfwShowWindow(s.window);

    glfwMakeContextCurrent(s.window);

    glfwDestroyWindow(window);

    while (!glfwWindowShouldClose(s.window))
    {
        //imgui_begin();
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
            }

            else if (currentScene->getName() == "MarcinScene") {
                // Wywołanie metody update dla exampleSceneScript
                //calibrationsceneScript->onExit();
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

            else if (currentScene->getName() == "SongScene") {
                // Wywołanie metody update dla exampleSceneScript
                songsceneScript->update();
            }
            else if (currentScene->getName() == "CalibrationScene") {
                // Wywołanie metody update dla exampleSceneScript
                calibrationsceneScript->update();
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