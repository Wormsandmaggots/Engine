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
    EditorLayer::Editor &editor;
    // collision
    CollisionManager &cm;
    // scene manager
    SceneManager &sm;
    // audio
    AudioManager &audioManager;
    // input joystick
    int connectedControllers;
    PlayerInput &playerInput;
    PlayerInput &playerInput1;
    glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;
    DebugInput &debugInput;

    Shader &shader;
    Shader &collisionTestShader;
    Shader &shaderText;
    Shader &colorShader;
    Shader &shaderPbr;
    Shader &screenShader;
    Shader &shaderRig;
    Shader &shaderBarmanRig;
    Shader &shaderDjRig;
    Shader &DrunkShader;
    Shader &shaderNoneDrink;
    Shader &reverseShader;
    Shader &imageShader;
    Shader &imageShaderGreen;
    Shader &shaderRigInstanced;
    Shader &shaderRigInstanced2;
    FrameBuffer buffer;

    // ssao
    SSAO &ssao;
    // renderer
    Renderer &renderer;
    Model *playerModel;

    RigPrep *playerRig;
    InverseKinematics *playerIK;

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
    calibrationSceneScript(EditorLayer::Editor &editor, CollisionManager &cm, SceneManager &sm, SSAO &ssao, Renderer &renderer, AudioManager &audioManager, PlayerInput &playerInput,
                       PlayerInput &playerInput1, DebugInput &debugInput, Shader &shader, Shader &collisionTestShader, Shader &shaderText,
                       Shader &colorShader, Shader &shaderPbr, Shader &screenShader, Shader &shaderRig, Shader &shaderBarmanRig, Shader &DrunkShader,
                       Shader &shaderNoneDrink, Shader &reverseShader, Shader &imageShader, Shader &imageShaderGreen, Shader &shaderRigInstanced, Shader &shaderDjRig,Shader &shaderRigInstanced2) : editor(editor),
                                             cm(cm),
                                             sm(sm),
                                             ssao(ssao),
                                             renderer(renderer),
                                             audioManager(audioManager),
                                             connectedControllers(JslConnectDevices()),
                                             playerInput(playerInput),
                                             playerInput1(playerInput1),
                                             debugInput(debugInput),
                                             joystickOffset(glm::vec2(0)),
                                             joystickOffset2(glm::vec2(0)),
                                             joystickOffset3(glm::vec2(0)),
                                             joystickOffset4(glm::vec2(0)),
                                             shader(shader),
                                             collisionTestShader(collisionTestShader),
                                             shaderText(shaderText),
                                             colorShader(colorShader),
                                             shaderPbr(shaderPbr),
                                             screenShader(screenShader),
                                             shaderRig(shaderRig),
                                             shaderBarmanRig(shaderBarmanRig),
                                             DrunkShader(DrunkShader),
                                             shaderNoneDrink(shaderNoneDrink),
                                             reverseShader(reverseShader),
                                             imageShader(imageShader),
                                             imageShaderGreen(imageShaderGreen),
                                             shaderRigInstanced(shaderRigInstanced),
                                             shaderDjRig(shaderDjRig),
                                             shaderRigInstanced2(shaderRigInstanced2),
                                             buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),
                                             playerModel(new Model("res/content/models/Chlop/MainCharacter.fbx", &shaderRig)),
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
                                             effectTime(10),
                                             timer(10),
                                             reversed(false),
                                             sun(new Entity("Sun")),
                                             sunLight(new DirectionalLight()),
                                             pointLight(new Entity("pointLight1")),
                                             pointLight1(new PointLight()),
                                             handOrbEntity(new Entity("handOrbEntity")),
                                             footOrbEntity(new Entity("footOrbEntity"))
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

        // lights
        pointLight->addComponent(pointLight1);
        pointLight->getTransform()->setScale(glm::vec3(5.f));
        currentScene->addEntity(pointLight);

        currentScene->addEntity(sun);
        sun->addComponent(sunLight);

        // gemplay
        player->addComponent(playerModel);
        player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
        player->getTransform()->setScale(glm::vec3(0.01f));
        currentScene->addEntity(player);

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

        glm::vec3 inactivePos = glm::vec3(5,5,5);
        glm::vec3 inactivePos2 = glm::vec3(-5,5,5);
        Collectable* handOrb = new HandOrb("handOrb", inactivePos, new Model(*handOrbModel));
        Collectable* footOrb = new FootOrb("footOrb", inactivePos2, new Model(*footOrbModel));

        handOrbEntity->addComponent(handOrbModel);
        handOrbEntity->getTransform()->setPosition(inactivePos);
        currentScene->addEntity(handOrbEntity);

        footOrbEntity->addComponent(footOrbModel);
        footOrbEntity->getTransform()->setPosition(inactivePos2);
        currentScene->addEntity(footOrbEntity);
        
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
        LightManager::UpdateLightShader(shaderRigInstanced, view);
        LightManager::UpdateLightShader(ssao.shaderGeometryPass, view);
        LightManager::UpdateLightShader(shaderBarmanRig, view);
        LightManager::UpdateLightShader(shaderRigInstanced2, view);
        LightManager::UpdateLightShader(shaderDjRig, view);

        ssao.shaderGeometryPass.use();
        renderer.updateProjectionAndView(projection, view, s.camera.Position);
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

        joystickOffset = playerInput.getJoystick(2);
        joystickOffset2 = playerInput.getJoystick(1);
        joystickOffset3 = playerInput1.getJoystick(2);
        joystickOffset4 = playerInput1.getJoystick(1);

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

        joystickOffset.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset2.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset2.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset2.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset2.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset3.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset3.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset3.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset3.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset4.x = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset4.x, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset4.y = Math::Remap(
            utils::easeInOutQuint(Math::Remap(joystickOffset4.y, -1, 1, 0, 1)),
            0, 1, -1, 1);

        joystickOffset *= 200 * s.deltaTime;
        joystickOffset2 *= 200 * s.deltaTime;
        joystickOffset3 *= 200 * s.deltaTime;
        joystickOffset4 *= 200 * s.deltaTime;
        // old

        playerIK->update(-joystickOffset3[0], -joystickOffset3[1], "mixamorig:RightHand");
        playerIK->update(-joystickOffset4[0], -joystickOffset4[1], "mixamorig:LeftHand");
        playerIK->update(-joystickOffset[0], -joystickOffset[1], "mixamorig:RightFoot");
        playerIK->update(-joystickOffset2[0], -joystickOffset2[1], "mixamorig:LeftFoot");
        playerRig->update();

        auto transforms = playerRig->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shaderRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        rightHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);
        leftHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);
        rightFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);
        leftFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);

    };

    void onExit() {
    };

    void onDestroy() override {

    };

    ~calibrationSceneScript() override = default;
};

#endif
