#ifndef ENGINE_CALIBRATIONSCENESCRIPT_H
#define ENGINE_CALIBRATIONSCENESCRIPT_H

#include "Script/SceneScript.h"

#include "windows.h"
#include "Text/Text.h"

#include "Engine/Engine.h"
#include "Core/AssetManager/AssetManager.h"
#include "ThirdPersonCamera.h"
#include "Physics/ColliderComponent.h"
#include "Physics/CollisionManager.h"
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
#include "Light/DirectionalLight.h"
#include "Light/LightManager.h"
#include "Light/PointLight.h"
#include "ForwardMovement.h"
#include "Generative-System/SpawnerComponent.h"
#include "Animation/LookAt.h"

using namespace SceneManagement;

class calibrationSceneScript : public SceneScript
{
private:
    // collision
    CollisionManager &cm;
    // scene manager
    SceneManager &sm;
    // audio
//    AudioManager &audioManager;
    // input joystick
    int connectedControllers;
    PlayerInput &playerInput;
    PlayerInput &playerInput1;
    glm::vec2 joystickOffsetCalibra;
    glm::vec2 joystickOffset2Calibra;
    glm::vec2 joystickOffset3Calibra;
    glm::vec2 joystickOffset4Calibra;
    DebugInput &debugInput;

    Shader &shader;
    Shader &collisionTestShader;
    Shader &shaderText;
    Shader &colorShader;
    Shader &shaderPbr;
    Shader &screenShader;
    Shader &shaderRig;
//    Shader &shaderBarmanRig;
//    Shader &shaderDjRig;
//    Shader &DrunkShader;
    //Shader &shaderNoneDrink;
//    Shader &reverseShader;
    Shader &imageShader;
    Shader &imageShaderGreen;
//    Shader &shaderRigInstanced;
//    Shader &shaderRigInstanced2;
    FrameBuffer buffer;

    // ssao
    SSAO &ssao;
    // renderer
    Renderer &renderer;
    Model *playerModelCalibra;

    RigPrep *playerRigCalibra;
    InverseKinematics *playerIKCalibra;

    bool reversed;

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

    float timeToDispense;
    float timeToDispense2;
    float effectTime;
    float timer;

    Entity *player;

    Entity *leftHandPointer;
    ColliderComponent *lHandcollider;
    Entity *rightHandPointer;
    ColliderComponent *rHandcollider;
    Entity *leftFootPointer;
    ColliderComponent *leftFootCollider;
    Entity *rightFootPointer;
    ColliderComponent *rightFootCollider;

    Entity* firstBallE;
    Entity* secondBallE;
    Entity* thirdBallE;
    Entity* fourthBallE;
    Entity* backgroundE;

    Entity* firstRingE;
    Entity* secondRingE;
    Entity* thirdRingE;
    Entity* fourthRingE;

    Model* firstBall;
    Model* secondBall;
    Model* thirdBall;
    Model* fourthBall;
    Model* background;

    Model* firstRing;
    Model* secondRing;
    Model* thirdRing;
    Model* fourthRing;

    bool firstCollision;
    bool secondCollision;
    bool thirdCollision;
    bool fourthCollision;


    Entity *sun;
    DirectionalLight *sunLight;

    Entity *pointLight;

    PointLight* pointLight1;

    Model* handOrbModel =new Model("res/content/models/orbUP/orbUP.fbx");
    Model* footOrbModel = new Model("res/content/models/orbDOWN/orbDown.fbx");

    Entity* handOrbEntity;
    Entity* footOrbEntity;

public:
    // Konstruktor domyślny
    calibrationSceneScript(CollisionManager &cm, SceneManager &sm, SSAO &ssao, Renderer &renderer, /*AudioManager &audioManager, */PlayerInput &playerInput,
                       PlayerInput &playerInput1, DebugInput &debugInput, Shader &shader, Shader &collisionTestShader, Shader &shaderText,
                       Shader &colorShader, Shader &shaderPbr, Shader &screenShader, Shader &shaderRig,/* Shader &shaderBarmanRig, Shader &DrunkShader,*/
                       /*Shader &shaderNoneDrink, *//*Shader &reverseShader,*/ Shader &imageShader, Shader &imageShaderGreen /*Shader &shaderRigInstanced, Shader &shaderDjRig,Shader &shaderRigInstanced2*/) :
                                             cm(cm),
                                             sm(sm),
                                             ssao(ssao),
                                             renderer(renderer),
                                             //audioManager(audioManager),
                                             connectedControllers(JslConnectDevices()),
                                             playerInput(playerInput),
                                             playerInput1(playerInput1),
                                             debugInput(debugInput),
                                             joystickOffsetCalibra(glm::vec2(0)),
                                             joystickOffset2Calibra(glm::vec2(0)),
                                             joystickOffset3Calibra(glm::vec2(0)),
                                             joystickOffset4Calibra(glm::vec2(0)),
                                             shader(shader),
                                             collisionTestShader(collisionTestShader),
                                             shaderText(shaderText),
                                             colorShader(colorShader),
                                             shaderPbr(shaderPbr),
                                             screenShader(screenShader),
                                             shaderRig(shaderRig),
//                                             shaderBarmanRig(shaderBarmanRig),
//                                             DrunkShader(DrunkShader),
                                             //shaderNoneDrink(shaderNoneDrink),
//                                             reverseShader(reverseShader),
                                             imageShader(imageShader),
                                             imageShaderGreen(imageShaderGreen),
//                                             shaderRigInstanced(shaderRigInstanced),
//                                             shaderDjRig(shaderDjRig),
//                                             shaderRigInstanced2(shaderRigInstanced2),
                                             buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),

                                             playerModelCalibra(new Model("res/content/models/Chlop/MainCharacter.fbx", &shaderRig)),
                                             playerRigCalibra(new RigPrep(playerModelCalibra)),
                                             playerIKCalibra(new InverseKinematics(playerRigCalibra)),
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
                                             timeToDispense2(timeToDispense),
                                             player(new Entity("Player")),
                                             leftHandPointer(new Entity("leftHandPointer")),
                                             lHandcollider(new ColliderComponent()),
                                             rightHandPointer(new Entity("rightHandPointer")),
                                             rHandcollider(new ColliderComponent()),
                                             leftFootPointer(new Entity("leftFootPointer")),
                                             leftFootCollider(new ColliderComponent()),
                                             rightFootPointer(new Entity("rightFootPointer")),
                                             rightFootCollider(new ColliderComponent()),
                                             backgroundE(new Entity("background")),
                                             effectTime(10),
                                             timer(10),
                                             reversed(false),
                                             sun(new Entity("Sun")),
                                             sunLight(new DirectionalLight()),
                                             pointLight(new Entity("pointLight1")),
                                             pointLight1(new PointLight()),
                                             handOrbEntity(new Entity("handOrbEntity")),
                                             footOrbEntity(new Entity("footOrbEntity")),
                                             firstBallE(new Entity("firstBallE")),
                                            secondBallE(new Entity("secondBallE")),
                                            thirdBallE(new Entity("thirdBallE")),
                                            fourthBallE(new Entity("fourthBallE")),
                                            firstRingE(new Entity("firstRingE")),
                                            secondRingE(new Entity("secondRingE")),
                                            thirdRingE(new Entity("thirdRing")),
                                            fourthRingE(new Entity("fourthRingE")),
                                            firstBall(new Model("res/content/models/orbUP/orbUP.fbx", &ssao.shaderGeometryPass)),
                                            secondBall(new Model("res/content/models/orbUP/orbUP.fbx", &ssao.shaderGeometryPass)),
                                            thirdBall(new Model("res/content/models/orbDown/orbDOWN.fbx", &ssao.shaderGeometryPass)),
                                            fourthBall(new Model("res/content/models/orbDown/orbDOWN.fbx", &ssao.shaderGeometryPass)),
                                            firstRing(new Model("res/content/models/orbUP/ringUP.fbx", &ssao.shaderGeometryPass)),
                                            secondRing(new Model("res/content/models/orbUP/ringUP.fbx", &ssao.shaderGeometryPass)),
                                            thirdRing(new Model("res/content/models/orbDown/ringDOWN.fbx", &ssao.shaderGeometryPass)),
                                            fourthRing(new Model("res/content/models/orbDown/ringDOWN.fbx", &ssao.shaderGeometryPass)),
                                            background(new Model("res/content/models/roofwalls/roof_walls.fbx", &ssao.shaderGeometryPass)),
                                            firstCollision(false),
                                            secondCollision(false),
                                            thirdCollision(false),
                                            fourthCollision(false)
    {
    }



    void awake() override
    {
        linear = 0.09f;
        quadratic = 0.032f;
        power = 1;
        kernelSize = 64;
        radius = 0.5f;
        bias = 0.025f;
        onlySSAO = true;
        range = glm::vec2(2, 2);
        mul = 4;
        texelSize = 1;
    };

    void start() override
    {
        sm.setCurrentScene("CalibrationScene");
        Scene2 *currentScene = sm.getSceneByName("CalibrationScene");

        screenShader.use();
        screenShader.setInt("screenTexture", 0);
/*
        // lights
        pointLight->addComponent(pointLight1);
        pointLight->getTransform()->setPosition(glm::vec3(1.f));
        pointLight->getTransform()->setScale(glm::vec3(74.0f, 7.0f, 100.0f));
*/
        currentScene->addEntity(pointLight);

        currentScene->addEntity(sun);
        sun->addComponent(sunLight);
        sun->getTransform()->setPosition(glm::vec3(0.0f, 10.0f,0.0f));

        // gemplay
        player->addComponent(playerModelCalibra);
        player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
        player->getTransform()->setScale(glm::vec3(0.01f));
        currentScene->addEntity(player);

        lHandcollider->start();
        lHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
        leftHandPointer->setParent(*player);
        leftHandPointer->addComponent(lHandcollider);
        leftHandPointer->getTransform()->setPosition(playerRigCalibra->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);

        rHandcollider->start();
        rHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
        rightHandPointer->setParent(*player);
        rightHandPointer->addComponent(rHandcollider);
        rightHandPointer->getTransform()->setPosition(playerRigCalibra->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);

        leftFootCollider->start();
        leftFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
        leftFootPointer->setParent(*player);
        leftFootPointer->addComponent(leftFootCollider);
        leftFootPointer->getTransform()->setPosition(playerRigCalibra->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);

        rightFootCollider->start();
        rightFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
        rightFootPointer->setParent(*player);
        rightFootPointer->addComponent(rightFootCollider);
        rightFootPointer->getTransform()->setPosition(playerRigCalibra->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);

        //orbs
        firstBallE->addComponent(firstBall);
        currentScene->addEntity(firstBallE);
        firstBallE->getTransform()->translate(rightHandPointer->getTransform()->getPosition());
        firstBallE->getTransform()->translate(glm::vec3(-0.5f, -2.0f, 100.0f));
        firstBallE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        firstBallE->getTransform()->setScale(glm::vec3(0.35f));

        firstRingE->addComponent(firstRing);
        currentScene->addEntity(firstRingE);
        firstRingE->getTransform()->translate(rightHandPointer->getTransform()->getPosition());
        firstRingE->getTransform()->translate(glm::vec3(-0.5f, -2.0f, 0.0f));
        firstRingE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        firstRingE->getTransform()->setScale(glm::vec3(0.35f));

        secondBallE->addComponent(secondBall);
        currentScene->addEntity(secondBallE);
        secondBallE->getTransform()->translate(leftHandPointer->getTransform()->getPosition());
        secondBallE->getTransform()->translate(glm::vec3(0.8f, -1.8f, 100.0f));
        secondBallE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        secondBallE->getTransform()->setScale(glm::vec3(0.35f));

        secondRingE->addComponent(secondRing);
        currentScene->addEntity(secondRingE);
        secondRingE->getTransform()->translate(leftHandPointer->getTransform()->getPosition());
        secondRingE->getTransform()->translate(glm::vec3(0.8f, -1.8f, -0.0f));
        secondRingE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        secondRingE->getTransform()->setScale(glm::vec3(0.35f));


        thirdBallE->addComponent(thirdBall);
        currentScene->addEntity(thirdBallE);
        thirdBallE->getTransform()->translate(rightFootPointer->getTransform()->getPosition());
        thirdBallE->getTransform()->translate(glm::vec3(-0.841f, -1.584f, 100.0f));
        thirdBallE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        thirdBallE->getTransform()->setScale(glm::vec3(0.35f));

        thirdRingE->addComponent(thirdRing);
        currentScene->addEntity(thirdRingE);
        thirdRingE->getTransform()->translate(rightFootPointer->getTransform()->getPosition());
        thirdRingE->getTransform()->translate(glm::vec3(-0.841f, -1.584f, 0.0f));
        thirdRingE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        thirdRingE->getTransform()->setScale(glm::vec3(0.35f));


        fourthBallE->addComponent(fourthBall);
        currentScene->addEntity(fourthBallE);
        fourthBallE->getTransform()->translate(leftFootPointer->getTransform()->getPosition());
        fourthBallE->getTransform()->translate(glm::vec3(1.111f, -0.952f, 100.0f));
        fourthBallE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        fourthBallE->getTransform()->setScale(glm::vec3(0.35f));

        fourthRingE->addComponent(fourthRing);
        currentScene->addEntity(fourthRingE);
        fourthRingE->getTransform()->translate(leftFootPointer->getTransform()->getPosition());
        fourthRingE->getTransform()->translate(glm::vec3(1.111f, -0.952f, -0.00f));
        fourthRingE->getTransform()->rotate(glm::vec3(-90.0f,180.0f,0.0f));
        fourthRingE->getTransform()->setScale(glm::vec3(0.35f));

        backgroundE->addComponent(background);
        currentScene->addEntity(backgroundE);
        backgroundE->getTransform()->setScale(glm::vec3(1.0f));
        backgroundE->getTransform()->rotate(glm::vec3(270.0f, 0.0f, 0.0f));
        backgroundE->getTransform()->setPosition(glm::vec3(0.0f, -23.0f, -69.0f));

        s.camera.SetPosition(glm::vec3(0.0f, -0.3f, -8.0f));
        LOG_INFO(std::to_string(s.camera.Position.x) + " " + std::to_string(s.camera.Position.y)+" " + std::to_string(s.camera.Position.z));
    };

    void update() override
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);

        time = time + s.deltaTime;

        deltaTime = s.deltaTime;
        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        LightManager::UpdateLightShader(shaderRig, view);
        LightManager::UpdateLightShader(ssao.shaderGeometryPass, view);

        ssao.shaderGeometryPass.use();
        renderer.updateProjectionAndView(projection, view, s.camera.Position);
        glBindFramebuffer(GL_FRAMEBUFFER, ssao.gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        for (unsigned int i = 0; i < 16; ++i)
        {
            if (i > kernelSize)
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
        buffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ssao.shaderLightingPass.use();
        ssao.shaderLightingPass.setVec3("camPos", s.camera.Position);
        LightManager::UpdateLightShader(ssao.shaderLightingPass, view);
        // send light relevant uniforms
        //        glm::vec3 lightPosView = glm::vec3(s.camera.GetViewMatrix() * glm::vec4(sphere1->getTransform()->getLocalPosition(), 1.0));
        //        ssao.shaderLightingPass.setVec3("light.Position", lightPosView);
        //        ssao.shaderLightingPass.setVec3("light.Color", lightColor);
        //// Update attenuation parameters
        //
        //
        //        ssao.shaderLightingPass.setFloat("light.Linear", linear);
        //        ssao.shaderLightingPass.setFloat("light.Quadratic", quadratic);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ssao.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.gAlbedo);
        glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBufferBlur);
        glActiveTexture(GL_TEXTURE4); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.gWorldPos);
        glActiveTexture(GL_TEXTURE5); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.gMetallicRoughnessAmbient);
        glActiveTexture(GL_TEXTURE6); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.gEmissive);
        ssao.renderQuad();

        joystickOffsetCalibra = playerInput1.getJoystick(2);
        joystickOffset2Calibra = playerInput1.getJoystick(1);
        joystickOffset3Calibra = playerInput.getJoystick(2);
        joystickOffset4Calibra = playerInput.getJoystick(1);

        buffer.unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, buffer.getTexture());
        ssao.renderQuad();

        glDisable(GL_DEPTH_TEST);
        // imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        cm.update();

        shaderRig.use();

        joystickOffsetCalibra.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffsetCalibra.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffsetCalibra.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffsetCalibra.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset2Calibra.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset2Calibra.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset2Calibra.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset2Calibra.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset3Calibra.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset3Calibra.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset3Calibra.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset3Calibra.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset4Calibra.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset4Calibra.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset4Calibra.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset4Calibra.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffsetCalibra *= 200 * s.deltaTime;
        joystickOffset2Calibra *= 200 * s.deltaTime;
        joystickOffset3Calibra *= 200 * s.deltaTime;
        joystickOffset4Calibra *= 200 * s.deltaTime;

        playerIKCalibra->update(-joystickOffset3Calibra[0], -joystickOffset3Calibra[1], "mixamorig:RightHand");
        playerIKCalibra->update(-joystickOffset4Calibra[0], -joystickOffset4Calibra[1], "mixamorig:LeftHand");
        playerIKCalibra->update(-joystickOffsetCalibra[0], -joystickOffsetCalibra[1], "mixamorig:RightFoot");
        playerIKCalibra->update(-joystickOffset2Calibra[0], -joystickOffset2Calibra[1], "mixamorig:LeftFoot");
        playerRigCalibra->update();

        auto transforms = playerRigCalibra->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shaderRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        rightHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRigCalibra->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);
        leftHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRigCalibra->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);
        rightFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRigCalibra->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);
        leftFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRigCalibra->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);

         if(glm::distance(glm::vec2(rightHandPointer->getTransform()->getPosition()), glm::vec2(firstRing->getTransform()->getPosition())+glm::vec2(0.0f,+2.5f))< 0.2f){
            if(firstCollision == false){
                AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 0.4);
            }
             firstCollision = true;
            firstBall->getTransform()->setPosition(glm::vec3(firstBall->getTransform()->getPosition().x,firstBall->getTransform()->getPosition().y,0.0f));
        }
        else{
            firstCollision = false;
             firstBall->getTransform()->setPosition(glm::vec3(firstBall->getTransform()->getPosition().x,firstBall->getTransform()->getPosition().y,100.0f));
        }

        if(glm::distance(glm::vec2(leftHandPointer->getTransform()->getPosition()), glm::vec2(secondRing->getTransform()->getPosition())+glm::vec2(0.0f,+2.5f))< 0.2f){
            if(secondCollision == false){
                AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 0.4);
            }
            secondCollision = true;
            secondBall->getTransform()->setPosition(glm::vec3(secondBall->getTransform()->getPosition().x,secondBall->getTransform()->getPosition().y,0.0f));
        }
        else{
            secondCollision = false;
            secondBall->getTransform()->setPosition(glm::vec3(secondBall->getTransform()->getPosition().x,secondBall->getTransform()->getPosition().y,100.0f));
        }

        if(glm::distance(glm::vec2(rightFootPointer->getTransform()->getPosition()), glm::vec2(thirdRing->getTransform()->getPosition())+glm::vec2(0.0f,+2.5f))< 0.2f){
            if(thirdCollision == false){
                AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 0.4);
            }
            thirdCollision = true;
            thirdBall->getTransform()->setPosition(glm::vec3(thirdBall->getTransform()->getPosition().x,thirdBall->getTransform()->getPosition().y,0.0f));
        }
        else{
            thirdCollision = false;
            thirdBall->getTransform()->setPosition(glm::vec3(thirdBall->getTransform()->getPosition().x,thirdBall->getTransform()->getPosition().y,100.0f));
        }

        if(glm::distance(glm::vec2(leftFootPointer->getTransform()->getPosition()), glm::vec2(fourthRing->getTransform()->getPosition())+glm::vec2(0.0f,+2.5f))< 0.2f){
            if(fourthCollision == false){
                AudioManager::getInstance().playSound("res/content/sounds/effects/pop1.wav", 0.4);
            }
            fourthCollision = true;
            fourthBall->getTransform()->setPosition(glm::vec3(fourthBall->getTransform()->getPosition().x,fourthBall->getTransform()->getPosition().y,0.0f));
        }
        else{
            fourthCollision = false;
            fourthBall->getTransform()->setPosition(glm::vec3(fourthBall->getTransform()->getPosition().x,fourthBall->getTransform()->getPosition().y,100.0f));
        }


        if(firstCollision || secondCollision || thirdCollision || fourthCollision){
            sm.setCurrentScene("MarcinScene");
        }

        if (playerInput.isKeyPressed(1))
        {
            sm.setCurrentScene("MarcinScene");
        }

    };

   /* void onExit() {
        if(playerModelCalibra != nullptr) {
            delete playerModelCalibra;
            playerModelCalibra = nullptr;
        }

        if(playerRigCalibra != nullptr) {
            delete playerRigCalibra;
            playerRigCalibra = nullptr;
        }

        if(playerIKCalibra != nullptr) {
            delete playerIKCalibra;
            playerIKCalibra = nullptr;
        }

//        if(player != nullptr) {
//            delete player;
//            player = nullptr;
//        }

        if(leftHandPointer != nullptr) {
            delete leftHandPointer;
            leftHandPointer = nullptr;
        }

        if(rightHandPointer != nullptr) {
            delete rightHandPointer;
            rightHandPointer = nullptr;
        }

        if(leftFootPointer != nullptr) {
            delete leftFootPointer;
            leftFootPointer = nullptr;
        }

        if(rightFootPointer != nullptr) {
            delete rightFootPointer;
            rightFootPointer = nullptr;
        }

//        if(sun != nullptr) {
//            delete sun;
//            sun = nullptr;
//        }

//        if(pointLight != nullptr) {
//            delete pointLight;
//            pointLight = nullptr;
//        }
//
//        if(handOrbEntity != nullptr) {
//            delete handOrbEntity;
//            handOrbEntity = nullptr;
//        }
//
//        if(footOrbEntity != nullptr) {
//            delete footOrbEntity;
//            footOrbEntity = nullptr;
//        }
    }
*/
    void onDestroy() override {

    };

    ~calibrationSceneScript() override = default;
};

#endif
