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
#include "Script/exampleSceneScript.h"
#include "Script/menuSceneScript.h"

#include "Globals.h"

using namespace SceneManagement;

int main() {
#pragma region INITIALIZERS
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
    sm.loadScene("res/content/maps/Marcin.yaml");

    sm.setCurrentScene("exampleScene");

//imp
    exampleSceneScript* exampleScene = new exampleSceneScript();
    menuSceneScript* menuScene = new menuSceneScript();

    //HID
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    DebugInput debugInput;
    //HID - test

    Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
    Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    Shader screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");
    Shader imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl");
#pragma endregion INITIALIZERS

    //HUD
#pragma region texures
    Texture menuBack("res/content/textures/backgg.jpg", "backgg");
    Texture menuVerticalColumn("res/content/textures/pole.png", "pole");
    Texture menuNewGame("res/content/textures/ng.png", "ng");
    Texture menuExit("res/content/textures/ex.png", "ex");
    Texture menuNewGameAlt("res/content/textures/nodes.png", "nodes");
#pragma endregion textures

#pragma region hudObjects
    Image* menuBackground = new Image(&imageShader);
    menuBackground->setTexture(&menuBack);

    Image* menuColumn = new Image(&imageShader);
    menuColumn->setTexture(&menuVerticalColumn);

    Button* newGame = new Button(&imageShader);
    newGame->setTexture(&menuNewGame);
    newGame->setOnClick([]() {
        std::cout << "start game" << std::endl;
    });

    Button* exit = new Button(&imageShader);
    exit->setTexture(&menuExit);
    exit->setOnClick([]() {
        std::cout << "wyjscie" << std::endl;
    });
#pragma endregion hudObjects

#pragma region Entities
    //main menu
//Image
    Entity* menuWalpaper = new Entity("menuBackground");
    sm.getLoadedScenes()[1]->addEntity(menuWalpaper);
    menuWalpaper->addComponent(menuBackground);
    menuBackground->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    menuBackground->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    Entity* menuAditional = new Entity("menuPole");
    sm.getLoadedScenes()[1]->addEntity(menuAditional);
    menuAditional->addComponent(menuColumn);
    menuColumn->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    menuColumn->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

//button
    Entity* ng = new Entity("newGame");
    sm.getLoadedScenes()[1]->addEntity(ng);
    ng->addComponent(newGame);
    newGame->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    newGame->getTransform()->setPosition(glm::vec3(-0.75f, -0.1f, 0.0f));

    Entity* ex = new Entity("exit");
    sm.getLoadedScenes()[1]->addEntity(ex);
    ex->addComponent(exit);
    exit->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    exit->getTransform()->setPosition(glm::vec3(-0.75f, -0.3f, 0.0f));
#pragma endregion Entities
    // Dodaj tekstury do wektora
    std::vector<Texture*> animationTextures = {&menuNewGame, &menuNewGameAlt};

    // Ustaw tekstury animacji dla przycisku
    newGame->setAnimationTextures(animationTextures);


    //for resizing bar
    double lastTime = 0.0;



#pragma region DUPA

    MaterialAsset material("res/content/materials/color.json");

    SSAO ssao;
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    Renderer renderer(&ssao.shaderGeometryPass);
    renderer.init();

    //renderer.addShader(&collisionTestShader);
    //renderer.addShader(&colorShader);
    //renderer.addShader(material.getShader());//TODO: Automatyczne dodawanie shadera do updatowania MVP

    Model* player = new Model("res/content/models/player/character_base.obj");

    /*renderer.addShader(&shaderText);
    renderer.addShader(&shaderPbr);
    renderer.addShader(&shaderCel);
    renderer.addShader(&ssao.shaderGeometryPass);*/

    Model* club = new Model("res/content/models/club2/club2.obj", &ssao.shaderGeometryPass);
    Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass);
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
    sphere1->addComponent(sphere);
    sphere->getTransform()->setPosition(lightPos);

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);
    player->getTransform()->setPosition(glm::vec3(-7, -2, 1));

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    //FrameBuffer* fb = new FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

    static float linear    = 0.09f;
    static float quadratic = 0.032f;
    static float power = 1;
    static float kernelSize = 64;
    static float radius = 0.5f;
    static float bias = 0.025f;
    static bool onlySSAO = true;
    static vec2 range(2,2);
    static float mul = 4;
    static float texelSize = 1;


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
    sound->setVolume(1.f);
#pragma endregion DUPA

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        //fb->bind();

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
        //glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
        //glm::mat4 view = playerCamera->getView();

       /*
       if (timeToDispense2 < 0 && songDataIndex < songData.size())
        {
            spawner.spawnBall("bass", glm::vec3(-songData[songDataIndex].bass.x, -songData[songDataIndex].bass.y, -20));
            spawner.spawnBall("mid", glm::vec3(-songData[songDataIndex].mid.x, songData[songDataIndex].mid.y, -20));
            spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, songData[songDataIndex].high.y, -20));
            spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, -songData[songDataIndex].high.y, -20));

            songDataIndex++;
            timeToDispense2 = timeToDispense;
        }

        ImGui::Begin("SSAO");

        {
            ImGui::DragFloat3("light Color", glm::value_ptr(lightColor));
            ImGui::DragFloat("linear", &linear);
            ImGui::DragFloat("quadratic", &quadratic);
            ImGui::DragFloat("power", &power);
            ImGui::DragFloat("kernelSize", &kernelSize);
            ImGui::DragFloat("radius", &radius);
            ImGui::DragFloat("bias", &bias);
            ImGui::DragFloat2("range", glm::value_ptr(range));
            ImGui::DragFloat("multiplier", &mul);
            ImGui::DragFloat("texel size", &texelSize);
            ImGui::Checkbox("Only SSAO", &onlySSAO);
        }

        ImGui::End();

        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
        ssao.shaderGeometryPass.use();*/
        renderer.updateProjectionAndView(projection, view);
        //glBindFramebuffer(GL_FRAMEBUFFER, ssao.gBuffer);
       // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        editor.draw();

        //imp
        sm.updateLoadedScenes();
        //scene switching
        if (glfwGetKey(s.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (sm.getCurrentScene()->getName() == "exampleScene") {
                sm.setCurrentScene("MarcinScene");
            } else {
                sm.setCurrentScene("exampleScene");
            }
        }
/*
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, ssao.ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssao.shaderSSAO.use();
        ssao.shaderSSAO.setFloat("power", power);
        ssao.shaderSSAO.setFloat("kernelSize", kernelSize);
        ssao.shaderSSAO.setFloat("radius", radius);
        ssao.shaderSSAO.setFloat("bias", bias);
        // Send kernel + rotation
        for (unsigned int i = 0; i < 64; ++i)
        {
            if(i > kernelSize)
            {
                ssao.shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", vec3(0));
            }
            else
                ssao.shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", ssao.ssaoKernel[i]);
        }

        ssao.shaderSSAO.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ssao.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.noiseTexture);
        ssao.renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, ssao.ssaoBlurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssao.shaderSSAOBlur.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBuffer);
        ssao.shaderSSAOBlur.setInt("rangeX", range.x);
        ssao.shaderSSAOBlur.setInt("rangeY", range.y);
        ssao.shaderSSAOBlur.setFloat("mul", mul);
        ssao.shaderSSAOBlur.setFloat("texelSize", texelSize);
        ssao.renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ssao.shaderLightingPass.use();
        // send light relevant uniforms
        glm::vec3 lightPosView = glm::vec3(s.camera.GetViewMatrix() * glm::vec4(sphere1->getTransform()->getLocalPosition(), 1.0));
        ssao.shaderLightingPass.setVec3("light.Position", lightPosView);
        ssao.shaderLightingPass.setVec3("light.Color", lightColor);
        // Update attenuation parameters


        ssao.shaderLightingPass.setFloat("light.Linear", linear);
        ssao.shaderLightingPass.setFloat("light.Quadratic", quadratic);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ssao.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.gAlbedo);
        glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBufferBlur);
        ssao.renderQuad();*/
        //scene.update();






        //TODO::MENU

        //HUD
        //HUD Menu
        glDisable(GL_DEPTH_TEST);
        imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        imageShader.use();
//kolejnosc renderowania ma znaczenie
//pierwszy renderowany obiekt bedzie pod spodem
//1
        menuBackground->renderPlane();
        menuColumn->renderPlane();

//2
        newGame->renderPlane();
        exit->renderPlane();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);



//TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////
        double mouseX, mouseY;
        glfwGetCursorPos(s.window, &mouseX, &mouseY);

        newGame->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        newGame->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

        exit->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        exit->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
//TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////
//HUD





        cm.update();
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //arcadeRenderer->renderText();

        //fb->unbind();

        //screenShader.use();
        //fb->drawQuad();


//        sm.updateLoadedScenes();
//
//        cm.update();
        //arcadeRenderer->update();
        update();
    }
    a.end();

    end();
    return 0;
}