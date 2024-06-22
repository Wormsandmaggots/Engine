#ifndef ENGINE_SONGSCENESCRIPT_H
#define ENGINE_SONGSCENESCRIPT_H

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

using namespace SceneManagement;

class songSceneScript : public SceneScript {
private:
///////////////////////////////////////////////////
    EditorLayer::Editor& editor;
    // collision
    CollisionManager& cm;
    // scene manager
    SceneManager& sm;
///////////////////////////////////////////////////
    // audio
    AudioManager& audioManager;

    float songSampleInterval;
    std::vector<SongSample> songData;
    int songDataIndex;

    // input joystick
    int connectedControllers;
///////////////////////////////////////////////////
    PlayerInput& playerInput;
    PlayerInput& playerInput1;
///////////////////////////////////////////////////
    glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;
///////////////////////////////////////////////////
    DebugInput& debugInput;

    Shader& shader;
    Shader& collisionTestShader;
    Shader& shaderText;
    Shader& colorShader;
    Shader& shaderPbr;
    Shader& screenShader;
    Shader& shaderRig;
    Shader& shaderBarmanRig;
    Shader& DrunkShader;
    Shader& shaderNoneDrink;
    Shader& reverseShader;
    Shader& imageShader;
    Shader& imageShaderGreen;
    Shader& shaderRigInstanced;
///////////////////////////////////////////////////
    FrameBuffer buffer;

    // ssao
    SSAO& ssao;
///////////////////////////////////////////////////
    // renderer
    Renderer& renderer;
///////////////////////////////////////////////////
    // model
    Model* box;
    Model* club;
    Model* sciany;
    Model* sphere;
    Model* player2;
    Model* playerModel;
    Model* barDrinks;
    Model* barman;
    Model* dj;

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

    // spawner
    //Spawner* spawner;
    Spawner* spawner = nullptr;
    float timeToDispense;
    float timeToDispense2;
    float effectTime;
    float timer;

    // entities
    Entity* clubE;
    Entity* scianyE;
    Entity* boxE;
    Entity* barDrinksE;
    Entity* barmanE;
    Entity* sphere1;
    Entity* player3;
    Entity* player;
    Entity* djE;
    Entity* leftHandPointer;
    ColliderComponent* lHandcollider;
    Entity* rightHandPointer;
    ColliderComponent* rHandcollider;
    Entity* leftFootPointer;
    ColliderComponent* leftFootCollider;
    Entity* rightFootPointer;
    ColliderComponent* rightFootCollider;
    const char* path;

    
    Entity* dancingRobots;
    Entity* dancingRobots2;
    InstancedRobots* ir;
    InstancedRobots* ir2;
    Animation* npcAnimation;
    Animator* npcAnimator;
    RigPrep* npcRig;

    Animation* barmanAnimation;
    Animator* barmanAnimator;
    RigPrep* barmanRig;

    Entity* sun;
    DirectionalLight* sunLight;

    Entity* pointLight;
    PointLight* pointLight1;

    //HUD
    double lastTime;
    ResizableImage* resBar;
    Entity* resBarEntity;

    ForwardMovement* fm;
    float z;

public:
    // Konstruktor domyślny
    songSceneScript(EditorLayer::Editor& editor, CollisionManager& cm, SceneManager& sm, SSAO& ssao, Renderer& renderer, AudioManager& audioManager, PlayerInput& playerInput,
                       PlayerInput& playerInput1, DebugInput& debugInput, Shader& shader, Shader& collisionTestShader, Shader& shaderText,
                       Shader& colorShader, Shader& shaderPbr, Shader& screenShader, Shader& shaderRig, Shader& shaderBarmanRig, Shader& DrunkShader,
                       Shader& shaderNoneDrink, Shader& reverseShader, Shader& imageShader, Shader& imageShaderGreen, Shader& shaderRigInstanced) :
            editor(editor),
            cm(cm),
            sm(sm),
            ssao(ssao),
            renderer(renderer),
            audioManager(audioManager),
            songSampleInterval(1.0),
            songDataIndex(0),
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
            //renderer(&ssao.shaderGeometryPass),
            buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),
            box(new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass)),
            club(new Model("res/content/models/klub/klubiec.fbx", &ssao.shaderGeometryPass)),
            sciany(new Model("res/content/models/club2/sciany.fbx", &ssao.shaderGeometryPass)),
            sphere(new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass)),
            barDrinks(new Model("res/content/models/kieliszki/drineczki_re.fbx",&ssao.shaderGeometryPass)),
            player2(new Model("res/content/models/npc/npcv2.fbx", &ssao.shaderGeometryPass)),
            barman(new Model("res/content/models/barman_rignorig/BARMAN_ANIMATIONv2.fbx", &shaderBarmanRig)),
            //barman(new Model("res/content/models/npc/npcv2.fbx", &shaderBarmanRig)),
            playerModel(new Model("res/content/models/Chlop/MainCharacter.fbx", &shaderRig)),
            sphereModel(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"))),
            sphereModel_green(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
            sphereModel_green2(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
            dj(new Model("res/content/models/mrDJ/noRig/MrDJ/MrDJ.fbx",&ssao.shaderGeometryPass)),
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
            scianyE(new Entity("sciany")),
            barDrinksE(new Entity("barDrinks")),
            barmanE(new Entity("barman")),
            sphere1(new Entity("sphere")),
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
            path("res/content/sounds/songs/if_you_dont.wav"),
            reversed(false),
            dancingRobots(new Entity("dancingRobots1")),
            dancingRobots2(new Entity("dancingRobots2")),
            ir(new InstancedRobots("res/content/models/npc/npcv2.fbx", glm::ivec2(5,5),
                                   &shaderRigInstanced,
                                   glm::vec3(-9.0f,-3.0f,0.0f), glm::vec3(70,0,70), glm::vec3(0.01f))),
            ir2(new InstancedRobots("res/content/models/npc/npcv2.fbx", glm::ivec2(5,5),
                                   &shaderRigInstanced,
                                   glm::vec3(6.0f,-3.0f,0.0f), glm::vec3(70,0,70), glm::vec3(0.01f))),
            npcAnimation(new Animation("res/content/models/npc/npcv2.fbx", ir)),
            npcAnimator(new Animator(npcAnimation)),
            npcRig(new RigPrep(ir)),
            barmanAnimation(new Animation("res/content/models/barman_rignorig/BARMAN_ANIMATIONv2.fbx", barman)),
            barmanAnimator(new Animator(barmanAnimation)),
            barmanRig(new RigPrep(barman)),
            sun(new Entity("Sun")),
            djE(new Entity("dj")),
            sunLight(new DirectionalLight()),
            pointLight(new Entity("pointLight1")),
            pointLight1(new PointLight()),
            fm(new ForwardMovement("res/content/sounds/songs/if_you_dont.wav",glm::vec3(0, -2.5, 0),glm::vec3(0, -2.5, 47))),
    //hud
    player3(new Entity("player3")),
    resBar(new ResizableImage(&imageShaderGreen)),
    resBarEntity(new Entity("resBar")),
    lastTime(0.0)
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
        z =5;
    };

    void start() override{

        //audio
        //audioManager.init();

        SongAnalizer::parseSong(songSampleInterval, path, songData);
        SongAnalizer::testparseSong(songSampleInterval, path, songData);

        //scene manager
        //sm.loadScene("res/content/maps/Marcin.yaml");
        sm.setCurrentScene("MarcinScene");

        // Inicjalizacja spawnera
        spawner = new Spawner(sm.getSceneByName("MarcinScene"));

        //ssao
        //ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

        //renderer
        //renderer.init();

        //screen shader
        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        //entities
        //club interior
        clubE->addComponent(club);
        sm.getSceneByName("MarcinScene")->addEntity(clubE);
        club->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        club->getTransform()->setScale(glm::vec3(0.5f));
        club->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        //player3->addComponent(player2);
        //sm.getSceneByName("MarcinScene")->addEntity(player3);
        //player3->getTransform()->setPosition(glm::vec3(2, -2.5, 0));
/*
        djE->addComponent(dj);
        sm.getSceneByName("MarcinScene")->addEntity(djE);
        dj->getTransform()->setPosition(glm::vec3(2, -2.5, 0));
*/
        scianyE->addComponent(sciany);
        sm.getSceneByName("MarcinScene")->addEntity(scianyE);
        sciany->getTransform()->setScale(glm::vec3(0.5f));
        sciany->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        barDrinksE->addComponent(barDrinks);
        sm.getSceneByName("MarcinScene")->addEntity(barDrinksE);
        barDrinks->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        barDrinks->getTransform()->setScale(glm::vec3(0.5f));
        barDrinks->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        barmanE->addComponent(barman);
        sm.getSceneByName("MarcinScene")->addEntity(barmanE);
        barman->getTransform()->setScale(glm::vec3(0.02f));
        barman->getTransform()->rotate(glm::vec3(0.0f,180.0f, 0.0f));
        barman->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,50.728f));

        //lights
        pointLight->addComponent(pointLight1);
        pointLight->getTransform()->setScale(glm::vec3(2000.f));
        sm.getSceneByName("MarcinScene")->addEntity(pointLight);


        sm.getSceneByName("MarcinScene")->addEntity(sun);
        sun->addComponent(sunLight);

        sm.getSceneByName("MarcinScene")->addEntity(dancingRobots);
        dancingRobots->addComponent(ir);

        sm.getSceneByName("MarcinScene")->addEntity(dancingRobots2);
        dancingRobots->addComponent(ir2);

        sm.getSceneByName("MarcinScene")->addEntity(sphere1);
        sphere1->addComponent(sphere);
        sphere->getTransform()->setPosition(lightPos);

        //movement
        sm.getSceneByName("MarcinScene")->addEntity(fm);
        fm->getTransform()->setPosition(glm::vec3(0, -2.5, 0));

        //gemplay
        player->addComponent(playerModel);
        player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
        player->getTransform()->setScale(glm::vec3(0.01f));
        sm.getSceneByName("MarcinScene")->addEntity(player);

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

        //hud
        sm.getSceneByName("MarcinScene")->addEntity(resBarEntity);
        resBarEntity->addComponent(resBar);
        resBar->getTransform()->setScale(glm::vec3(0.02f, 0.3f, 0.0f));
        resBar->getTransform()->setPosition(glm::vec3(0.847f, 0.0f, 0.0f));

//txt
        comboRenderer->setParameters("Combo " + std::to_string(combo) + "x", 150, 950, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
        scoreRenderer->setParameters("Score " + std::to_string(score), 1920/2 - 12, 950, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);


        //AudioManager::getInstance().playSound(path, 1.0f);

        DrunkShader.setInt("screenTexture", 0);
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

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        barmanAnimator->UpdateAnimation(deltaTime);

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();


        //moving forward
        if(deltaTime < 1){
            player->getTransform()->translate(glm::vec3(0.0f,0.0f,deltaTime*globalVelocity));
            z += deltaTime * globalVelocity;

        }

// bardziej randomowe spawnowanie
        timeToDispense2 -= s.deltaTime;
        if (timeToDispense2 < 0 && songDataIndex < songData.size()) {

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

                    //nozki
                    if (spawner->hasXPercentChance(20))
                        spawner->spawnBadBall("ball", glm::vec3(1, -2, z));
                    else 
                        spawner->spawnBall("ball", glm::vec3(1, -2, z));
                    if (spawner->hasXPercentChance(20)) 
                        spawner->spawnBadBall("ball", glm::vec3(-1, -2, z));
                    else 
                        spawner->spawnBall("ball", glm::vec3(-1, -2, z));
                    

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
                    if (spawner->hasXPercentChance(20)) 
                        spawner->spawnBadBall("ball", glm::vec3(0.8, -0.8, z));
                    else 
                        spawner->spawnBall("ball", glm::vec3(0.8, -0.8, z));
                  

                    if (spawner->hasXPercentChance(20)) 
                        spawner->spawnBadBall("ball", glm::vec3(-0.8, -0.8, z));
                  
                    else 
                        spawner->spawnBall("ball", glm::vec3(-0.8, -0.8, z));
                    

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

                    //nozki
                    
                    if (spawner->hasXPercentChance(20)) 
                        spawner->spawnBadBall("ball", glm::vec3(0.2, -2.25, z));
                    else
                        spawner->spawnBall("ball", glm::vec3(0.2, -2.25, z));

                    if (spawner->hasXPercentChance(20))
                        spawner->spawnBadBall("ball", glm::vec3(-0.2, -2.25, z));
                    else
                        spawner->spawnBall("ball", glm::vec3(-0.2, -2.25, z));

                              
                 
                    break;
                case sampleType::SKIP:
                    break;
            }
           if (spawner->ballsSpawned % 50 == 0 && spawner->ballsSpawned != 0)
                spawner->spawnDrink("drink", glm::vec3(-1, 1, z));

            songDataIndex++;
            timeToDispense2 = timeToDispense;

            if (!(songDataIndex < songData.size())) songDataIndex = 0;
        }       

        

        npcAnimator->UpdateAnimation(s.deltaTime);

        shaderRigInstanced.use();
        npcRig->update();

        auto transforms2 = npcAnimator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms2.size(); ++i)
            shaderRigInstanced.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms2[i]);

        //npcRig->update();
        shaderBarmanRig.use();
        barmanRig->update();
        auto transforms3 = barmanAnimator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms3.size(); ++i)
            shaderBarmanRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms3[i]);

        LightManager::UpdateLightShader(shaderRig, view);
        LightManager::UpdateLightShader(shaderRigInstanced, view);
        LightManager::UpdateLightShader(ssao.shaderGeometryPass, view);
        LightManager::UpdateLightShader(shaderBarmanRig, view);

        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
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
//scene.update();
       
        joystickOffset = playerInput.getJoystick(2);
        joystickOffset2 = playerInput.getJoystick(1);
        joystickOffset3 = playerInput1.getJoystick(2);
        joystickOffset4 = playerInput1.getJoystick(1);

        if (timer < 0) {
            timer = effectTime;
			currentDrink = DrinkType::None;
		}

        buffer.unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        switch (currentDrink) {
        case DrinkType::Drunk:
            DrunkShader.use();
            DrunkShader.setFloat("time", time);
            DrunkShader.setInt("screenTexture", 0);
            timer -= s.deltaTime;

            break;
        case DrinkType::InverseInput:
            shaderNoneDrink.use();
            shaderNoneDrink.setInt("screenTexture", 0);

            joystickOffset = -joystickOffset;
            joystickOffset2 = -joystickOffset2;
            joystickOffset3 = -joystickOffset3;
            joystickOffset4 = -joystickOffset4;
            timer -= s.deltaTime;

            break;
        case DrinkType::UpsideDown:

            reverseShader.use();
            reverseShader.setFloat("time", time);
            reverseShader.setInt("screenTexture", 0);
            timer -= s.deltaTime;

            break;
        case DrinkType::None:
            shaderNoneDrink.use();
            shaderNoneDrink.setInt("screenTexture", 0);
            break;
        }


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, buffer.getTexture());
        ssao.renderQuad();


        glDisable(GL_DEPTH_TEST);
        //imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //hud
        resBar->renderPlane();
        //resizing bar
        //temporary------------------------------------------------------------------------------------
        double currentTime = glfwGetTime();
        // Jeśli upłynęła 1 sekunda od ostatniej aktualizacji
        if (currentTime - lastUpdateTime >= resizeInterval) {
            resBar->resizeOnImpulse(resizeAmount);
            lastUpdateTime = currentTime;
        }
        // Jeśli score został zwiększony o incrementScore
        if (score - lastScore >= incrementScore) {
            resBar->increaseOnImpulse(resizeAmount);
            lastScore = score;
        }
//        if (resBar->getTransform()->getLocalScale().y <= 0.01f) {
//            std::cout << "Koniec" << std::endl;
//        }

        

        //temporary------------------------------------------------------------------------------------
        //text
        comboRenderer->setParameters("Combo " + std::to_string(combo) + "x", 150, 950, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
        scoreRenderer->setParameters("Score " + std::to_string(score), 1920/2 - 12, 950, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);

        comboRenderer->renderText();
        scoreRenderer->renderText();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        spawner->update();

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

    };

    void onDestroy() override{
        //audioManager.end();
        delete spawner;
    };

    ~songSceneScript() override = default;
};


#endif
