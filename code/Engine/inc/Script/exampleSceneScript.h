#ifndef ENGINE_EXAMPLESCENESCRIPT_H
#define ENGINE_EXAMPLESCENESCRIPT_H

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
#include "Light/SpotLight.h"
#include "ForwardMovement.h"
#include "Animation/LookAt.h"

using namespace SceneManagement;

class exampleSceneScript : public SceneScript {
private:
    EditorLayer::Editor editor;
    // collision
    CollisionManager cm;
    // scene manager
    SceneManager sm;

    // audio
    AudioManager& audioManager;

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
    Shader shaderBarmanRig;
    Shader shaderDjRig;
    Shader DrunkShader;
    Shader shaderNoneDrink;
    Shader reverseShader;
    Shader imageShader;
    Shader imageShaderGreen;

    FrameBuffer buffer;

    Shader shaderRigInstanced;
    Shader shaderRigInstanced2;

    // ssao
    SSAO ssao;

    // renderer
    Renderer renderer;

    // model
    Model* box;
    Model* club;
    Model* sciany;
    Model* player2;
    Model* playerModel;
    Model* barDrinks;
    Model* canisters;
    Model* barman;
    Model* drinkBarman;
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
    Entity* canistersE;
    Entity* barmanE;
    Entity* drinkBarmanE;
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
    Animation* npcAnimation2;
    Animator* npcAnimator;
    Animator* npcAnimator2;

    Animation* barmanAnimation;
    Animator* barmanAnimator;
    RigPrep* barmanRig;

    Animation* djAnimation;
    Animator* djAnimator;

    Entity* sun;
    DirectionalLight* sunLight;

    Entity* pointLight;
    Entity* pointLight2E;
    Entity* pointLight3E;
    Entity* pointLight4E;
    Entity* pointLight5E;
    PointLight* pointLight1;
    PointLight* pointLight2;
    PointLight* pointLight3;
    PointLight* pointLight4;
    PointLight* pointLight5;

    Entity* spotLight1E;
    SpotLight* spotLight1;
    //HUD
    double lastTime;
    ResizableImage* resBar;
    Entity* resBarEntity;

    ForwardMovement* fm;
    float z;

public:
    // Konstruktor domyślny
    exampleSceneScript() :
            audioManager(AudioManager::getInstance()),
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
            shaderBarmanRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            shaderDjRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag"),
            DrunkShader("res/content/shaders/SSAO/ssao.vert", "res/content/shaders/chromaticAberration.frag"),
            shaderNoneDrink("res/content/shaders/SSAO/ssao.vert", "res/content/shaders/framebuffer.frag"),
            reverseShader("res/content/shaders/SSAO/ssao.vert","res/content/shaders/reverse.frag"),
            imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl"),
            imageShaderGreen("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_green.glsl"),
            renderer(&ssao.shaderGeometryPass),
            buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),
            box(new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass)),
            club(new Model("res/content/models/klub/klubiec2.fbx", &ssao.shaderGeometryPass)),
            sciany(new Model("res/content/models/roofwalls/roof_walls.fbx", &ssao.shaderGeometryPass)),
            canisters(new Model("res/content/models/Canister/Canister/kanistry.fbx", &ssao.shaderGeometryPass)),
            barDrinks(new Model("res/content/models/kieliszki/drineczki_re.fbx",&ssao.shaderGeometryPass)),
            player2(new Model("res/content/models/npc/npcv2.fbx", &ssao.shaderGeometryPass)),
            barman(new Model("res/content/models/barman_rignorig/BARMAN_ANIMATIONv2.fbx", &shaderBarmanRig)),
            drinkBarman(new Model("res/content/models/kieliszki/drink1/drink1_re.fbx", &ssao.shaderGeometryPass)),
            playerModel(new Model("res/content/models/Chlop/MainCharacter.fbx", &shaderRig)),
            sphereModel(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"))),
            sphereModel_green(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
            sphereModel_green2(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
            dj(new Model("res/content/models/mrDJ/noRig/MrDJ/DJ.fbx",&shaderDjRig)),
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
            canistersE(new Entity("canisters")),
            barmanE(new Entity("barman")),
            drinkBarmanE(new Entity("barmanDrink")),
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
            shaderRigInstanced(Shader("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/SSAO/ssao_fragment.frag")),
            shaderRigInstanced2(Shader("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/SSAO/ssao_fragment.frag")),
            ir(new InstancedRobots("res/content/models/npc/npc23.fbx", glm::ivec2(5,5),
                                   &shaderRigInstanced,
                                   glm::vec3(-11.0f,-3.0f,0.0f), glm::vec3(150,0,300), glm::vec3(0.01f))),
            ir2(new InstancedRobots("res/content/models/npc/npc23.fbx", glm::ivec2(5,5),
                                   &shaderRigInstanced2,
                                   glm::vec3(5.0f,-3.0f,0.0f), glm::vec3(150,0,300), glm::vec3(0.01f))),
            npcAnimation(new Animation("res/content/models/npc/npc23.fbx", ir)),
            npcAnimation2(new Animation("res/content/models/npc/npc23.fbx", ir2)),
            npcAnimator(new Animator(npcAnimation,true)),
            npcAnimator2(new Animator(npcAnimation,true)),
            barmanAnimation(new Animation("res/content/models/barman_rignorig/BARMAN_ANIMATIONv2.fbx", barman)),
            barmanAnimator(new Animator(barmanAnimation, false)),
            barmanRig(new RigPrep(barman)),
            djAnimation(new Animation("res/content/models/mrDJ/noRig/MrDJ/DJ.fbx", dj)),
            djAnimator(new Animator(djAnimation, false)),
            sun(new Entity("Sun")),
            djE(new Entity("dj")),
            sunLight(new DirectionalLight()),
            pointLight(new Entity("pointLight1")),
            pointLight1(new PointLight()),
            pointLight2E(new Entity("pointLight2bar")),
            pointLight2(new PointLight()),
            pointLight3E(new Entity("pointLight3bar")),
            pointLight3(new PointLight()),
            pointLight4E(new Entity("pointLight4dj")),
            pointLight4(new PointLight()),
            pointLight5E(new Entity("pointLight5bar")),
            pointLight5(new PointLight()),
            spotLight1E(new Entity("spotLight1")),
            spotLight1(new SpotLight()),
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
        editor.init(&s.camera);
        z =5;
    };

    void start() override{

        //audio
        audioManager.init();

        SongAnalizer::parseSong(songSampleInterval, path, songData);
        SongAnalizer::testparseSong(songSampleInterval, path, songData);

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
        //club interior

        clubE->addComponent(club);
        sm.getLoadedScenes()[0]->addEntity(clubE);
        club->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        club->getTransform()->setScale(glm::vec3(0.5f));
        club->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        //player3->addComponent(player2);
        //sm.getLoadedScenes()[0]->addEntity(player3);
        //player3->getTransform()->setPosition(glm::vec3(2, -2.5, 0));

        djE->addComponent(dj);
        sm.getLoadedScenes()[0]->addEntity(djE);
        //dj->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        dj->getTransform()->setScale(glm::vec3(0.005f));
        dj->getTransform()->setPosition(glm::vec3(0.0f,-2.5f,0.0f));

        scianyE->addComponent(sciany);
        sm.getLoadedScenes()[0]->addEntity(scianyE);
        sciany->getTransform()->setScale(glm::vec3(0.5f));
        sciany->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        sciany->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        barDrinksE->addComponent(barDrinks);
        sm.getLoadedScenes()[0]->addEntity(barDrinksE);
        barDrinks->getTransform()->setScale(glm::vec3(0.005f));
        barDrinks->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        canistersE->addComponent(canisters);
        sm.getLoadedScenes()[0]->addEntity(canistersE);
        canisters->getTransform()->setScale(glm::vec3(0.5f));
        canisters->getTransform()->rotate(glm::vec3(270.0f,0.0f,0.0f));
        canisters->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,0.0f));

        barmanE->addComponent(barman);
        sm.getLoadedScenes()[0]->addEntity(barmanE);
        barman->getTransform()->setScale(glm::vec3(0.02f));
        barman->getTransform()->rotate(glm::vec3(0.0f,180.0f, 0.0f));
        barman->getTransform()->setPosition(glm::vec3(0.0f,-3.4f,50.728f));

        //lights
        pointLight->addComponent(pointLight1);
        pointLight->getTransform()->setScale(glm::vec3(5.f));
        sm.getLoadedScenes()[0]->addEntity(pointLight);

        pointLight2E->addComponent(pointLight2);
        pointLight2E->getTransform()->setScale(glm::vec3(74.0f,7.0f,100.0f));
        pointLight2E->getTransform()->setPosition(glm::vec3(6.0f,-0.7f,52.0f));
        sm.getLoadedScenes()[0]->addEntity(pointLight2E);

        pointLight3E->addComponent(pointLight3);
        pointLight3E->getTransform()->setScale(glm::vec3(20.0f,135.0f,245.0f));
        pointLight3E->getTransform()->setPosition(glm::vec3(-5.0f,-0.7f,52.0f));
        sm.getLoadedScenes()[0]->addEntity(pointLight3E);

        pointLight4E->addComponent(pointLight4);
        pointLight4E->getTransform()->setScale(glm::vec3(74.0f,7.0f,100.0f));
        pointLight4E->getTransform()->setPosition(glm::vec3(-2.5f,3.0f,53.7f));
        sm.getLoadedScenes()[0]->addEntity(pointLight4E);

        pointLight5E->addComponent(pointLight5);
        pointLight5E->getTransform()->setScale(glm::vec3(20.0f,135.0f,245.0f));
        pointLight5E->getTransform()->setPosition(glm::vec3(3.0f,3.0f,53.7f));
        sm.getLoadedScenes()[0]->addEntity(pointLight5E);

        spotLight1E->addComponent(spotLight1);
        spotLight1E->getTransform()->setScale(glm::vec3(100.0f));
        sm.getLoadedScenes()[0]->addEntity(spotLight1E);


        sm.getLoadedScenes()[0]->addEntity(sun);
        sun->addComponent(sunLight);


        sm.getLoadedScenes()[0]->addEntity(dancingRobots);
        dancingRobots->addComponent(ir);

        sm.getLoadedScenes()[0]->addEntity(dancingRobots2);
        dancingRobots->addComponent(ir2);


        //movement
        sm.getLoadedScenes()[0]->addEntity(fm);
        fm->getTransform()->setPosition(glm::vec3(0, -2.5, 0));

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

        //hud
        sm.getLoadedScenes()[0]->addEntity(resBarEntity);
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

        barmanAnimator->UpdateAnimation(deltaTime, 90.0f);
        djAnimator->UpdateAnimation(deltaTime, 90.0f);

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


        npcAnimator->UpdateAnimation(s.deltaTime, lookatAngle );
        npcAnimator2->UpdateAnimation(s.deltaTime, lookatAngle * (-1.0f));
        shaderRigInstanced.use();
        auto transforms2 = npcAnimator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms2.size(); ++i)
            shaderRigInstanced.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms2[i]);
        shaderRigInstanced2.use();
        auto transforms5 = npcAnimator2->GetFinalBoneMatrices();
        for (int i = 0; i < transforms5.size(); ++i)
            shaderRigInstanced2.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms5[i]);

        shaderBarmanRig.use();
        auto transforms3 = barmanAnimator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms3.size(); ++i)
            shaderBarmanRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms3[i]);

        shaderDjRig.use();
        auto transforms4 = djAnimator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms4.size(); ++i)
            shaderDjRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms4[i]);

        LightManager::UpdateLightShader(shaderRig, view);
        LightManager::UpdateLightShader(shaderRigInstanced, view);
        LightManager::UpdateLightShader(shaderRigInstanced2, view);
        LightManager::UpdateLightShader(ssao.shaderGeometryPass, view);
        LightManager::UpdateLightShader(shaderBarmanRig, view);
        LightManager::UpdateLightShader(shaderDjRig, view);

        //shaderPbr.use();
        //shaderPbr.setVec3("camPos",s.camera.Position);
        //shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
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
            if(lookatAngle <170.0f) {
                lookatAngle += 5.0f;
            }
        }
        // Jeśli score został zwiększony o incrementScore
        if (score - lastScore >= incrementScore) {
            resBar->increaseOnImpulse(resizeAmount);
            lastScore = score;
            if(lookatAngle > 5.0f){
                lookatAngle -=5.0f;
            }
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
        audioManager.end();
        delete spawner;
    };

    ~exampleSceneScript() override = default;
};


#endif
