#ifndef ENGINE_EXAMPLESCENESCRIPT_H
#define ENGINE_EXAMPLESCENESCRIPT_H

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
#include "Globals.h"


using namespace SceneManagement;

class exampleSceneScript : public SceneScript {
private:
    // editor
    EditorLayer::Editor editor;
    // collision
    CollisionManager cm;
    // scene manager
    SceneManager sm;

    // audio
    AudioManager& audioManager;
    std::shared_ptr<Sound> sound;
    std::shared_ptr<Sound> success;
    std::shared_ptr<Sound> failure;

    float songSampleInterval;
    std::vector<SongSample> songData;
    int songDataIndex;

    // input joystick
    int connectedControllers;

    PlayerInput playerInput;
    PlayerInput playerInput1;

    glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;

    DebugInput debugInput;

    Shader shader;
    Shader collisionTestShader;
    Shader shaderText;
    Shader colorShader;
    Shader shaderPbr;
    Shader screenShader;
    Shader shaderRig;

    // ssao
    SSAO ssao;

    // renderer
    Renderer renderer;

    // model
    Model* box;
    Model* club;
    Model* sphere;
    Model* player2;
    Model* playerModel;

    Model* sphereModel;
    Model* sphereModel_green;
    Model* sphereModel_green2;

    // text
    Text* comboRenderer;
    Text* scoreRenderer;

    // camera
    ThirdPersonCamera* playerCamera;

    // IK
    RigPrep* playerRig;
    InverseKinematics* playerIK;

    // light
    glm::vec3 lightPos;
    glm::vec3 lightColor;

    float linear;
    float quadratic;
    float power;
    float kernelSize;
    float radius;
    float bias;
    bool onlySSAO;
    glm::vec2 range;
    float mul;
    float texelSize;

    float time;

    // spawner
    //Spawner* spawner;
    Spawner* spawner = nullptr;
    float timeToDispense;
    float timeToDispense2;

    // entities
    Entity* clubE;
    Entity* boxE;
    Entity* sphere1;
    //Entity* player3;
    Entity* player;
    Entity* leftHandPointer;
    ColliderComponent* lHandcollider;
    Entity* rightHandPointer;
    ColliderComponent* rHandcollider;
    Entity* leftFootPointer;
    ColliderComponent* leftFootCollider;
    Entity* rightFootPointer;
    ColliderComponent* rightFootCollider;


public:
    // Konstruktor domyślny
    exampleSceneScript() :
            audioManager(AudioManager::getInstance()),
            sound(audioManager.loadSound("res/content/sounds/songs/queen.wav")),
            success(audioManager.loadSound("res/content/sounds/effects/clap.wav")),
            failure(audioManager.loadSound("res/content/sounds/effects/sweep.wav")),
            songSampleInterval(1.0),
            songDataIndex(0),
            connectedControllers(JslConnectDevices()),
            playerInput(GLFW_JOYSTICK_1),
            playerInput1(GLFW_JOYSTICK_2),
            joystickOffset(glm::vec2(0)),
            joystickOffset2(glm::vec2(0)),
            joystickOffset3(glm::vec2(0)),
            joystickOffset4(glm::vec2(0)),
            shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl"),
            collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag"),
            shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl"),
            colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl"),
            shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl"),
            screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag"),
            shaderRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            renderer(&ssao.shaderGeometryPass),
            box(new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass)),
            club(new Model("res/content/models/klub/KLUBv2.fbx", &ssao.shaderGeometryPass)),
            sphere(new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass)),
            //player2(new Model("res/content/models/barman/barman_animated.fbx", &ssao.shaderGeometryPass)),
            playerModel(new Model("res/content/models/Chlop/Main_character.fbx", &shaderRig)),
            sphereModel(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"))),
            sphereModel_green(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
            sphereModel_green2(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
            comboRenderer(new Text("res/content/fonts/ARCADECLASSIC.TTF")),
            scoreRenderer(new Text("res/content/fonts/ARCADECLASSIC.TTF")),
            playerCamera(new ThirdPersonCamera()),
            playerRig(new RigPrep(playerModel)),
            playerIK(new InverseKinematics(playerRig)),
            lightPos(glm::vec3(2.0, 4.0, -2.0)),
            lightColor(glm::vec3(0.2, 0.2, 0.7)),
            linear(0.09f),
            quadratic(0.032f),
            power(1),
            kernelSize(64),
            radius(0.5f),
            bias(0.025f),
            onlySSAO(true),
            range(glm::vec2(2, 2)),
            mul(4),
            texelSize(1),
            time(0),
            spawner(nullptr),
            timeToDispense(songSampleInterval),
            timeToDispense2(timeToDispense),
            clubE(new Entity("club")),
            boxE(new Entity("box")),
            sphere1(new Entity("sphere")),
            player(new Entity("Player")),
            leftHandPointer(new Entity("leftHandPointer")),
            lHandcollider(new ColliderComponent()),
            rightHandPointer(new Entity("rightHandPointer")),
            rHandcollider(new ColliderComponent()),
            leftFootPointer(new Entity("leftFootPointer")),
            leftFootCollider(new ColliderComponent()),
            rightFootPointer(new Entity("rightFootPointer")),
            rightFootCollider(new ColliderComponent())
    {
    }

    void awake() override{
        //turn into static
        linear    = 0.09f;
        quadratic = 0.032f;
        power = 1;
        kernelSize = 64;
        radius = 0.5f;
        bias = 0.025f;
        onlySSAO = true;
        range = glm::vec2(2,2);
        mul = 4;
        texelSize = 1;
    };

    void start() override{
        //editor
        editor.init(&s.camera);

        //audio
        audioManager.init();

        success->setVolume(0.2);
        failure->setVolume(0.5);

        SongAnalizer::parseSong(songSampleInterval, "res/content/sounds/songs/queen.wav", songData);
        SongAnalizer::testparseSong(songSampleInterval, "res/content/sounds/songs/queen.wav", songData);

        //scene manager
        sm.loadScene("res/content/maps/Marcin.yaml");
        sm.setCurrentScene("MarcinScene");

        // Inicjalizacja spawnera
        spawner = new Spawner(sm.getLoadedScenes()[0]);

        //ssao
        ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

        //renderer
        renderer.init();

        //screen shader
        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        //entities
/*
        clubE->addComponent(club);
        sm.getLoadedScenes()[0]->addEntity(clubE);
        club->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        club->getTransform()->setScale(glm::vec3(0.5f));
        club->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));
*/

        sm.getLoadedScenes()[0]->addEntity(sphere1);
        sphere1->addComponent(sphere);
        sphere->getTransform()->setPosition(lightPos);

        //gemplay
        player->addComponent(playerModel);
        player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
        player->getTransform()->setScale(glm::vec3(0.01f));
        sm.getLoadedScenes()[0]->addEntity(player);

        lHandcollider->start();
        lHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
        leftHandPointer->setParent(*player);
        leftHandPointer->addComponent(lHandcollider);
        leftHandPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);

        rHandcollider->start();
        rHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
        rightHandPointer->setParent(*player);
        rightHandPointer->addComponent(rHandcollider);
        rightHandPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);

        leftFootCollider->start();
        leftFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
        leftFootPointer->setParent(*player);
        leftFootPointer->addComponent(leftFootCollider);
        leftFootPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);

        rightFootCollider->start();
        rightFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
        rightFootPointer->setParent(*player);
        rightFootPointer->addComponent(rightFootCollider);
        rightFootPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);

        AudioManager::getInstance().playSound("res/content/sounds/songs/queen.wav", 1.0f);

    };

    void update() override{

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


// bardziej randomowe spawnowanie
        timeToDispense2 -= s.deltaTime;
        if (timeToDispense2 < 0 && songDataIndex < songData.size()) {
            float z = 5;
            switch (songData[songDataIndex].type) {
                case sampleType::BASS:
                    //raczki

                    if (spawner->hasXPercentChance(20)) {
                        spawner->spawnBadBall("ball", glm::vec3(1* songData[songDataIndex].bass.x, 1.5* songData[songDataIndex].bass.y, z));
                    }
                    else {
                        spawner->spawnBall("ball", glm::vec3(1*songData[songDataIndex].bass.x, 1.5* songData[songDataIndex].bass.y, z));
                    }

                    if (spawner->hasXPercentChance(20)) {

                        spawner->spawnBadBall("ball", glm::vec3(-1*songData[songDataIndex].bass.y, 1.5 * songData[songDataIndex].bass.x, z));
                    }
                    else {
                        spawner->spawnBall("ball", glm::vec3(-1 * songData[songDataIndex].bass.y, 1.5 * songData[songDataIndex].bass.x, z));
                    }

                    ////nozki
                    //if (spawner->hasXPercentChance(20)) {
                    //    spawner->spawnBadBall("ball", glm::vec3(1, -2, z));
                    //}
                    //else {
                    //    spawner->spawnBall("ball", glm::vec3(1, -2, z));
                    //}
                    //if (spawner->hasXPercentChance(20)) {
                    //    spawner->spawnBadBall("ball", glm::vec3(-1, -2, z));
                    //}
                    //else {
                    //    spawner->spawnBall("ball", glm::vec3(-1, -2, z));
                    //}

                    break;
                case sampleType::MID:
                    //raczki
                    if (spawner->hasXPercentChance(20)) {
                        spawner->spawnBadBall("ball", glm::vec3(1.55 * songData[songDataIndex].mid.x, 0.16 * songData[songDataIndex].mid.y, z));
                    }
                    else {
                        spawner->spawnBall("ball", glm::vec3(1.55 * songData[songDataIndex].mid.x, 0.16 * songData[songDataIndex].mid.y, z));
                    }

                    if (spawner->hasXPercentChance(20)) {

                        spawner->spawnBadBall("ball", glm::vec3(-1.55 * songData[songDataIndex].mid.y, 0.16 * songData[songDataIndex].mid.x, z));
                    }
                    else {
                        spawner->spawnBall("ball", glm::vec3(-1.55* songData[songDataIndex].mid.y, 0.16 * songData[songDataIndex].mid.x, z));
                    }
                    ////nozki
                    //if (spawner->hasXPercentChance(20)) {
                    //    spawner->spawnBadBall("ball", glm::vec3(0.8, -0.8, z));
                    //}
                    //else {
                    //    spawner->spawnBall("ball", glm::vec3(0.8, -0.8, z));
                    //}

                    //if (spawner->hasXPercentChance(20)) {

                    //    spawner->spawnBadBall("ball", glm::vec3(-0.8, -0.8, z));
                    //}
                    //else {
                    //    spawner->spawnBall("ball", glm::vec3(-0.8, -0.8, z));
                    //}

                    break;
                case sampleType::CLAP:
                    //raczki
                    if (spawner->hasXPercentChance(20)) {
                        spawner->spawnBadBall("ball", glm::vec3(1.1*songData[songDataIndex].high.x, -0.5 * songData[songDataIndex].high.y, z));
                    }
                    else {
                        spawner->spawnBall("ball", glm::vec3(1.1 * songData[songDataIndex].high.x, -0.5 * songData[songDataIndex].high.y, z));
                    }

                    if (spawner->hasXPercentChance(20)) {

                        spawner->spawnBadBall("ball", glm::vec3(-1.1 * songData[songDataIndex].high.y, -0.5 * songData[songDataIndex].high.x, z));
                    }
                    else {
                        spawner->spawnBall("ball", glm::vec3(-1.1 * songData[songDataIndex].high.y, -0.5 * songData[songDataIndex].high.x, z));
                    }

                    //////nozki
                    //          if (spawner->hasXPercentChance(20)) {
                    //              spawner->spawnBadBall("ball", glm::vec3(0.2, -2.25, z));

                    //          }
                    //          else {
                    //              spawner->spawnBall("ball", glm::vec3(0.2, -2.25, z));
                    //          }

                    //          if (spawner->hasXPercentChance(20)) {

                    //              spawner->spawnBadBall("ball", glm::vec3(-0.2, -2.25, z));
                    //          }
                    //          else {
                    //              spawner->spawnBall("ball", glm::vec3(-0.2, -2.25, z));
                    //          }

                    break;
                case sampleType::SKIP:
                    break;
            }
            if (spawner->ballsSpawned % 50 == 0 && spawner->ballsSpawned != 0) {
                spawner->spawnDrink("drink", glm::vec3(-1, 1, 5));
            }
            songDataIndex++;
            timeToDispense2 = timeToDispense;

            if (!(songDataIndex < songData.size())) songDataIndex = 0;
        }

        switch (currentDrink) {
        case DrinkType::Drunk:
            break;
        case DrinkType::InverseInput:
            break;
        case DrinkType::UpsideDown:
            break;
        case DrinkType::None:
            break;
        }

        shaderRig.use();
        joystickOffset = playerInput.getJoystick(2);
        joystickOffset2 = playerInput.getJoystick(1);
        joystickOffset3 = playerInput1.getJoystick(2);
        joystickOffset4 = playerInput1.getJoystick(1);

        joystickOffset.x = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset.x, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset.y = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset.y, -1, 1, 0 ,1)),
                0, 1, -1, 1);


        joystickOffset2.x = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset2.x, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset2.y = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset2.y, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset3.x = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset3.x, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset3.y = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset3.y, -1, 1, 0 ,1)),
                0, 1, -1, 1);


        joystickOffset4.x = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset4.x, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset4.y = Math::Remap(
                utils::easeInOutQuint(Math::Remap(joystickOffset4.y, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset *= 200 * s.deltaTime;
        joystickOffset2 *= 200 * s.deltaTime;
        joystickOffset3 *= 200 * s.deltaTime;
        joystickOffset4 *= 200 * s.deltaTime;
        //old

        playerIK->update(-joystickOffset[0], -joystickOffset[1], "mixamorig:RightHand");
        playerIK->update(-joystickOffset2[0], -joystickOffset2[1], "mixamorig:LeftHand");
        playerIK->update(-joystickOffset3[0], -joystickOffset3[1], "mixamorig:RightFoot");
        playerIK->update(-joystickOffset4[0], -joystickOffset4[1], "mixamorig:LeftFoot");
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

        spawner->update();

        cm.update();

    };

    void onDestroy() override{
        audioManager.end();
        delete spawner;
    };

    ~exampleSceneScript() override = default;
};


#endif
