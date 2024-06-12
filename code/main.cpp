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
#include "Script/SceneScript.h"
#include "Script/menuSceneScript.h"
#include "Script/exampleSceneScript.h"



#include "Globals.h"

using namespace SceneManagement;

int main() {
    int connectedControllers = JslConnectDevices();
    init();
    EditorLayer::Editor editor;
    editor.init(&s.camera);
    CollisionManager cm;
    AudioManager a;
    a.init();
    Sound* sound = a.loadSound("res/content/sounds/Ich will.mp3");
    SceneManager sm;

    glViewport(0,0, 1920, 1080);

    sm.loadScene("res/content/maps/test.yaml");
    sm.loadScene("res/content/maps/Marcin.yaml");

    sm.setCurrentScene("exampleScene");


    //HID
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    DebugInput debugInput;
    //HID - test

    Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
    Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");

    MaterialAsset material("res/content/materials/color.json");

    SSAO ssao;
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    Renderer renderer(&ssao.shaderGeometryPass);
    renderer.init();

    //renderer.addShader(&collisionTestShader);
    //renderer.addShader(&colorShader);
    //renderer.addShader(material.getShader());//TODO: Automatyczne dodawanie shadera do updatowania MVP

    Model* player = new Model("res/content/models/player/character_base.obj");

    Model* box = new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass);
    Model* club = new Model("res/content/models/club2/club2.obj", &ssao.shaderGeometryPass);
    Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass);
    Model* player2 = new Model("res/content/models/barman/barman_animated.fbx", &ssao.shaderGeometryPass);

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    cc1->start();
    cc2->start();

    Entity* clubE = new Entity("club");
    clubE->addComponent(club);
    sm.getLoadedScenes()[0]->addEntity(clubE);

    Entity* boxE = new Entity("box");
    boxE->addComponent(box);
    sm.getLoadedScenes()[0]->addEntity(boxE);

    Entity* player1 = new Entity("player");
    sm.getLoadedScenes()[0]->addEntity(player1);
    player1->addComponent(player);
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);

    Entity* sphere1 = new Entity("sphere");
    sm.getLoadedScenes()[0]->addEntity(sphere1);
    sphere1->addComponent(sphere);
    sphere->getTransform()->setPosition(lightPos);

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);
    player->getTransform()->setPosition(glm::vec3(-7, -2, 1));

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

    Model* sphereModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/material.json"));

    Entity ent("doopa");
    ent.addComponent(sphereModel);

    sm.getLoadedScenes()[0]->addEntity(&ent);

    float time = 0;

    sound->play();
    sound->setVolume(1.f);

//main scene script-----------------------------------------------------------------------------------------------
    exampleSceneScript ess(sm, lightColor, shaderPbr, sphere, sphere1, ssao, renderer, editor);
    ess.start();
//main scene script-----------------------------------------------------------------------------------------------

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        /*
        if (glfwGetKey(s.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (sm.getCurrentScene()->getName() == "exampleScene") {
                sm.setCurrentScene("MarcinScene");
            } else {
                sm.setCurrentScene("exampleScene");
            }
        }
         */

        //main scene script-----------------------------------------------------------------------------------------------
        ess.update(projection, view);
        //main scene script-----------------------------------------------------------------------------------------------


        cm.update();
        update();
    }
    a.end();

    end();
    return 0;
}