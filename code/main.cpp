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
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Light/LightManager.h"

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

    float songSampleInterval = 1;
    vector<SongSample> songData;
    SongAnalizer::parseSong(songSampleInterval, "res/content/sounds/queen.wav", songData);
    int songDataIndex = 0;

    glViewport(0,0, 1920, 1080);

    sm.loadScene("res/content/maps/test.yaml");

    //HID
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    DebugInput debugInput;
    //HID - test

    Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
    Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    //Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    Shader screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");

    //HUD

    MaterialAsset material("res/content/materials/color.json");

    SSAO ssao;
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    Renderer renderer(&ssao.shaderGeometryPass);
    renderer.init();

    Model* player = new Model("res/content/models/player/character_base.obj");

    Model* box = new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass);
    Model* club = new Model("res/content/models/club2/club2.obj", &ssao.shaderGeometryPass);
    //Model* sphere = new Model("res\\content\\models\\NEWCHARACTER\\untitled.obj", &ssao.shaderGeometryPass);
    //Model* player = new Model("res\\content\\models\\player\\character_base.obj", &shaderPbr);
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

    /*Entity* club1 = new Entity("club");
    sm.getLoadedScenes()[0]->addEntity(club1);
    club1->addComponent(club);
    club->getTransform()->setPosition(glm::vec3(0, -5, 0));
    club->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));*/

    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);

    Entity* sphere1 = new Entity("sphere");
    sm.getLoadedScenes()[0]->addEntity(sphere1);
    //sphere1->addComponent(sphere);
    sphere1->getTransform()->setPosition(lightPos);

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);
    player->getTransform()->setPosition(glm::vec3(-7, -2, 1));

    Entity* sun = new Entity("sun");
    sm.getLoadedScenes()[0]->addEntity(sun);
    sun->addComponent(new DirectionalLight());

    Entity* pointLight = new Entity("pointLight");
    sm.getLoadedScenes()[0]->addEntity(pointLight);
    pointLight->addComponent(new PointLight());

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    //FrameBuffer* fb = new FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

    static float linear    = 0.09f;
    static float quadratic = 0.032f;

    //Model* sphereModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
    Model* sphereModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/material.json"));

    Entity ent("doopa");
    ent.addComponent(sphereModel);
    sm.getLoadedScenes()[0]->addEntity(&ent);

    float time = 0;

    Spawner spawner(sm.getLoadedScenes().at(0));
    float timeToDispense = songSampleInterval;
    float timeToDispense2 = timeToDispense;

    sound->play();
    sound->setVolume(0.f);

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

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
        //glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
        //glm::mat4 view = playerCamera->getView();

        if (timeToDispense2 < 0 && songDataIndex < songData.size())
        {
            spawner.spawnBall("bass", glm::vec3(-songData[songDataIndex].bass.x, -songData[songDataIndex].bass.y, -20));
            spawner.spawnBall("mid", glm::vec3(-songData[songDataIndex].mid.x, songData[songDataIndex].mid.y, -20));
            spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, songData[songDataIndex].high.y, -20));
            spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, -songData[songDataIndex].high.y, -20));

            songDataIndex++;
            timeToDispense2 = timeToDispense;
        }

        renderer.updateProjectionAndView(projection, view);

        ssao.geometryBuffer();

        editor.draw();

        sm.updateLoadedScenes();

        ssao.ssaoBuffer();

        ssao.blurBuffer();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ssao.shaderLightingPass.use();
        LightManager::UpdateLightShader(ssao.shaderLightingPass, view);
        // send light relevant uniforms
//        glm::vec3 lightPosView = glm::vec3(s.camera.GetViewMatrix() * glm::vec4(sphere1->getTransform()->getLocalPosition(), 1.0));
//        ssao.shaderLightingPass.setVec3("pointLight.position", lightPosView);
//        ssao.shaderLightingPass.setVec3("pointLight.color", lightColor);
        ssao.shaderLightingPass.setVec3("camPos", s.camera.Position);
//        // Update attenuation parameters
//
//        ssao.shaderLightingPass.setFloat("pointLight.linear", linear);
//        ssao.shaderLightingPass.setFloat("pointLight.quadratic", quadratic);

//        ssao.shaderLightingPass.setVec3("spotLight.position", sphere1->getTransform()->getLocalPosition());
//        ssao.shaderLightingPass.setVec3("spotLight.direction", glm::normalize(sphere1->getTransform()->getLocalRotation()));
//        ssao.shaderLightingPass.setFloat("spotLight.cutoff", glm::radians(12.5f));
//        ssao.shaderLightingPass.setFloat("spotLight.outercutoff", glm::radians(17.5f));


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ssao.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.gAlbedo);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBufferBlur);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, ssao.gWorldPos);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, ssao.gMetallicRoughnessAmbient);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, ssao.gEmissive);

        ssao.renderQuad();

        cm.update();

        update();
    }

    a.end();

    end();
    return 0;
}