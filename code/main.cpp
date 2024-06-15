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
#include "Globals.h"
#include "Input/DebugInput.h"
#include "Input/PlayerInput.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "RigPrep.h"
#include "Animation/InverseKinematics.h"
#include "ECS/ScriptableEntity.h"

using namespace SceneManagement;

int main() {
    int connectedControllers = JslConnectDevices();
    init();
    EditorLayer::Editor editor;
    editor.init(&s.camera);
    CollisionManager cm;
    AudioManager& audioManager = AudioManager::getInstance();
    audioManager.init();
    std::shared_ptr<Sound> sound = audioManager.loadSound("res/content/sounds/songs/queen.wav");
    std::shared_ptr<Sound> success = audioManager.loadSound("res/content/sounds/effects/clap.wav");
    std::shared_ptr<Sound> failure = audioManager.loadSound("res/content/sounds/effects/sweep.wav");

    success->setVolume(0.2);
	failure->setVolume(0.5);

    SceneManager sm;

    float songSampleInterval = 0.3;
    vector<SongSample> songData;
    SongAnalizer::parseSong(songSampleInterval, "res/content/sounds/songs/queen.wav", songData);
    SongAnalizer::testparseSong(songSampleInterval, "res/content/sounds/songs/queen.wav", songData);

    int songDataIndex = 0;

    glViewport(0,0, 1920, 1080);

    PlayerInput playerInput(GLFW_JOYSTICK_1);
    PlayerInput playerInput1(GLFW_JOYSTICK_2);

    glm::vec2 joystickOffset = glm::vec2(0);
    glm::vec2 joystickOffset2 = glm::vec2(0);
    glm::vec2 joystickOffset3 = glm::vec2(0);
    glm::vec2 joystickOffset4 = glm::vec2(0);

    sm.loadScene("res/content/maps/test.yaml");

    Scene2* currentScene = sm.getLoadedScenes().at(0);


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
    Shader shaderRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/fragment.glsl");

    SSAO ssao;
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    Renderer renderer(&ssao.shaderGeometryPass);
    renderer.init();


    Model* box = new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass);
    Model* club = new Model("res/content/models/klub/KLUBv2.fbx", &ssao.shaderGeometryPass);
    Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass);
    Model* player2 = new Model("res/content/models/barman/barman_animated.fbx", &ssao.shaderGeometryPass);

    Text* comboRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* scoreRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    Model* playerModel = new Model("res/content/models/Chlop/Main_character.fbx", &shaderRig);
    RigPrep* playerRig = new RigPrep(playerModel);
    InverseKinematics* playerIK = new InverseKinematics(playerRig);

/*
    Entity* clubE = new Entity("club");
    clubE->addComponent(club);
    sm.getLoadedScenes()[0]->addEntity(clubE);
    club->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
    club->getTransform()->scale(glm::vec3(0.001f));

    Entity* boxE = new Entity("box");
    boxE->addComponent(box);
    sm.getLoadedScenes()[0]->addEntity(boxE);
*/
    

    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);

    Entity* sphere1 = new Entity("sphere");
    sm.getLoadedScenes()[0]->addEntity(sphere1);
    sphere1->addComponent(sphere);
    sphere->getTransform()->setPosition(lightPos);

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);

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



    float time = 0;

    Spawner spawner(sm.getLoadedScenes().at(0));
    float timeToDispense = songSampleInterval;
    float timeToDispense2 = timeToDispense;

    Model* sphereModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
    Model* sphereModel_green = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"));
    Model* sphereModel_green2 = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"));


    Entity* player = new Entity("Player");
    player->addComponent(playerModel);
    player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
    player->getTransform()->setScale(glm::vec3(0.01f));
    currentScene->addEntity(player);


    Entity* leftHandPointer = new Entity("leftHandPointer");
    ColliderComponent* lHandcollider = new ColliderComponent();
    lHandcollider->start();
    lHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
    leftHandPointer->setParent(*player);
    leftHandPointer->addComponent(lHandcollider);
    leftHandPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);

    Entity* rightHandPointer = new Entity("rightHandPointer");
    ColliderComponent* rHandcollider = new ColliderComponent();
    rHandcollider->start();
    rHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
    rightHandPointer->setParent(*player);
    rightHandPointer->addComponent(rHandcollider);
    rightHandPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);

    Entity* leftFootPointer = new Entity("leftFootPointer");
    ColliderComponent* leftFootCollider = new ColliderComponent();
    leftFootCollider->start();
    leftFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
    leftFootPointer->setParent(*player);
    leftFootPointer->addComponent(leftFootCollider);
    leftFootPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);

    Entity* rightFootPointer = new Entity("rightFootPointer");
    ColliderComponent* rightFootCollider = new ColliderComponent();
    rightFootCollider->start();
    rightFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
    rightFootPointer->setParent(*player);
    rightFootPointer->addComponent(rightFootCollider);
    rightFootPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);

  
    //sound->play();
    sound->setVolume(1.f);

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();

        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        time = time + s.deltaTime;
        deltaTime = s.deltaTime;

        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
        //glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
        //glm::mat4 view = playerCamera->getView();
/*
        timeToDispense2 -= s.deltaTime;
        if (timeToDispense2 < 0 && songDataIndex < songData.size())
        {
            spawner.spawnBall("bass", glm::vec3(-songData[songDataIndex].bass.x, -songData[songDataIndex].bass.y, -20));
            spawner.spawnBall("mid", glm::vec3(-songData[songDataIndex].mid.x, songData[songDataIndex].mid.y, -20));
            spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, songData[songDataIndex].high.y, -20));
            spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, -songData[songDataIndex].high.y, -20));

            songDataIndex++;
            timeToDispense2 = timeToDispense;
        }
*/
        shaderRig.use();

        joystickOffset2 = playerInput.getJoystick(2) * 100.0f;
        playerIK->update(-joystickOffset2[0], -joystickOffset2[1], "mixamorig:RightHand");
        playerRig->update();


        joystickOffset = playerInput.getJoystick(1) * 100.0f;
        playerIK->update(-joystickOffset[0], -joystickOffset[1], "mixamorig:LeftHand");
        playerRig->update();

        joystickOffset3 = playerInput1.getJoystick(1) * 100.0f;
        playerIK->update(-joystickOffset3[0], -joystickOffset3[1], "mixamorig:LeftFoot");
        playerRig->update();

        joystickOffset4 = playerInput1.getJoystick(2) * 100.0f;
        playerIK->update(-joystickOffset4[0], -joystickOffset4[1], "mixamorig:RightFoot");
        playerRig->update();

        auto transforms = playerRig->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shaderRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        rightHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);
        leftHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);
        rightFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);
        leftFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);


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
        ssao.shaderGeometryPass.use();
        renderer.updateProjectionAndView(projection, view);
        glBindFramebuffer(GL_FRAMEBUFFER, ssao.gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        editor.draw();

        sm.updateLoadedScenes();
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
        ssao.renderQuad();
        //scene.update();

        comboRenderer->setParameters("Score " + std::to_string(score), 100, 100, 1.0f, glm::vec3(0.6, 0.9f, 0.3f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);
        scoreRenderer->setParameters("Combo " + std::to_string(combo) + "x", 100, 150, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);

        comboRenderer->renderText();
        scoreRenderer->renderText();
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
    audioManager.end();

    end();
    return 0;
}