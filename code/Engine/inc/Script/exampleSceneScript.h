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
#include "ForwardMovement.h"
#include "Generative-System/SpawnerComponent.h"
#include "Animation/LookAt.h"

using namespace SceneManagement;

class exampleSceneScript : public SceneScript
{
private:
    ///////////////////////////////////////////////////
    EditorLayer::Editor &editor;
    // collision
    CollisionManager &cm;
    // scene manager
    SceneManager &sm;
    ///////////////////////////////////////////////////
    // audio
    AudioManager &audioManager;

    float songSampleInterval;
    std::vector<SongSample> songData;
    int songDataIndex;

    // input joystick
    int connectedControllers;
    ///////////////////////////////////////////////////
    PlayerInput &playerInput;
    PlayerInput &playerInput1;
    ///////////////////////////////////////////////////
    glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;
    ///////////////////////////////////////////////////
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
    ///////////////////////////////////////////////////
    FrameBuffer buffer;

    // ssao
    SSAO &ssao;
    ///////////////////////////////////////////////////
    // renderer
    Renderer &renderer;
    ///////////////////////////////////////////////////
    // model
    Model *box;
    Model *club;
    Model *sciany;
    Model *sphere;
    Model *player2;
    Model *playerModel;
    Model *barDrinks;
    Model *canisters;
    Model *barman;
    Model *dj;
    Model *chairs;

    Model *sphereModel;
    Model *sphereModel_green;
    Model *sphereModel_green2;

    // text
    Text *comboRenderer;
    Text *scoreRenderer;

    // camera
    ThirdPersonCamera *playerCamera;

    // IK
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

    // spawner
    // Spawner* spawner;
    // Spawner* spawner = nullptr;
    float timeToDispense;
    float timeToDispense2;
    float effectTime;
    float timer;

    // entities
    Entity *clubE;
    Entity *scianyE;
    Entity *boxE;
    Entity *barDrinksE;
    Entity *canistersE;
    Entity *barmanE;
    Entity *sphere1;
    Entity *player3;
    Entity *player;
    Entity *djE;
    Entity *chairsE;
    Entity *leftHandPointer;
    ColliderComponent *lHandcollider;
    Entity *rightHandPointer;
    ColliderComponent *rHandcollider;
    Entity *leftFootPointer;
    ColliderComponent *leftFootCollider;
    Entity *rightFootPointer;
    ColliderComponent *rightFootCollider;

    Entity *dancingRobots;
    Entity *dancingRobots2;
    Entity *dancingRobots3;
    Entity *dancingRobots4;
    InstancedRobots *ir;
    InstancedRobots *ir2;
    InstancedRobots *ir3;
    InstancedRobots *ir4;
    Animation *npcAnimation;
    Animator *npcAnimator;
    Animator *npcAnimator2;

    Animation *barmanAnimation;
    Animator *barmanAnimator;

    Animation *djAnimation;
    Animator *djAnimator;

    Entity *sun;
    DirectionalLight *sunLight;

    Entity *pointLight;
    Entity *pointLight2E;
    Entity *pointLight3E;
    Entity *pointLight4E;
    Entity *pointLight5E;
    Entity *pointLight6E;
    Entity *pointLight7E;
    Entity *pointLight8E;

    PointLight *pointLight1;
    PointLight *pointLight2;
    PointLight *pointLight3;
    PointLight *pointLight4;
    PointLight *pointLight5;
    PointLight *pointLight6;
    PointLight *pointLight7;
    PointLight *pointLight8;

    // HUD
    double lastTime;
    ResizableImage *resBar;
    Entity *resBarEntity;

    ForwardMovement *fm;
    float z;

    Entity *spawner;
    SpawnerComponent *spawnerComponent;

    Button *activeButton;

    // stoping bar from falling
    bool canDecreaseBar;
    float clock;
    float fallStop;

    //hud2
    Entity* scoreEntity;
    Image* scoreImage;
    Texture* scoreTexture;
    Entity* comboEntity;
    Image* comboImage;
    Texture* comboTexture;
    Entity* humanEntity;
    Image* humanImage;
    Texture* humanTexture;
    Entity* robotEntity;
    Image* robotImage;
    Texture* robotTexture;
    Entity* frameEntity;
    Image* frameImage;
    Texture* frameTexture;

public:
    // Konstruktor domyślny
    exampleSceneScript(EditorLayer::Editor &editor, CollisionManager &cm, SceneManager &sm, SSAO &ssao, Renderer &renderer, AudioManager &audioManager, PlayerInput &playerInput,
                       PlayerInput &playerInput1, DebugInput &debugInput, Shader &shader, Shader &collisionTestShader, Shader &shaderText,
                       Shader &colorShader, Shader &shaderPbr, Shader &screenShader, Shader &shaderRig, Shader &shaderBarmanRig, Shader &DrunkShader,
                       Shader &shaderNoneDrink, Shader &reverseShader, Shader &imageShader, Shader &imageShaderGreen, Shader &shaderRigInstanced, Shader &shaderDjRig, Shader &shaderRigInstanced2) : editor(editor),
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
                                                                                                                                                                                                      shaderDjRig(shaderDjRig),
                                                                                                                                                                                                      shaderRigInstanced2(shaderRigInstanced2),
                                                                                                                                                                                                      // renderer(&ssao.shaderGeometryPass),
                                                                                                                                                                                                      buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),
                                                                                                                                                                                                      box(new Model("res/content/models/box/box.obj", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      club(new Model("res/content/models/klub/klubiec2.fbx", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      sciany(new Model("res/content/models/roofwalls/roof_walls.fbx", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      sphere(new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      barDrinks(new Model("res/content/models/kieliszki/drineczki_re.fbx", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      canisters(new Model("res/content/models/Canister/Canister/kanistry.fbx", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      chairs(new Model("res/content/models/krzesla/krzeslo/krzesla.fbx", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      player2(new Model("res/content/models/npc/npcv2.fbx", &ssao.shaderGeometryPass)),
                                                                                                                                                                                                      barman(new Model("res/content/models/barman_rignorig/BARMAN_ANIMATIONv2.fbx", &shaderBarmanRig)),
                                                                                                                                                                                                      playerModel(new Model("res/content/models/Chlop/MainCharacter.fbx", &shaderRig)),
                                                                                                                                                                                                      sphereModel(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"))),
                                                                                                                                                                                                      sphereModel_green(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
                                                                                                                                                                                                      sphereModel_green2(new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"))),
                                                                                                                                                                                                      dj(new Model("res/content/models/mrDJ/noRig/MrDJ/DJ.fbx", &shaderDjRig)),
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
                                                                                                                                                                                                      // spawner(nullptr),
                                                                                                                                                                                                      timeToDispense(songSampleInterval),
                                                                                                                                                                                                      timeToDispense2(timeToDispense),
                                                                                                                                                                                                      clubE(new Entity("club")),
                                                                                                                                                                                                      boxE(new Entity("box")),
                                                                                                                                                                                                      scianyE(new Entity("sciany")),
                                                                                                                                                                                                      barDrinksE(new Entity("barDrinks")),
                                                                                                                                                                                                      canistersE(new Entity("canisters")),
                                                                                                                                                                                                      chairsE(new Entity("chairs")),
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
                                                                                                                                                                                                      // path("res/content/sounds/songs/if_you_dont.wav"),
                                                                                                                                                                                                      reversed(false),
                                                                                                                                                                                                      dancingRobots(new Entity("dancingRobots1")),
                                                                                                                                                                                                      dancingRobots2(new Entity("dancingRobots2")),
                                                                                                                                                                                                      dancingRobots3(new Entity("dancingRobots3")),
                                                                                                                                                                                                      dancingRobots4(new Entity("dancingRobots4")),
                                                                                                                                                                                                      ir(new InstancedRobots("res/content/models/npc/npc23.fbx", glm::ivec2(5, 15),
                                                                                                                                                                                                                             &shaderRigInstanced,
                                                                                                                                                                                                                             glm::vec3(-12.0f, -3.0f, 0.0f), glm::vec3(250, 0, 400), glm::vec3(0.008f))),
                                                                                                                                                                                                      ir2(new InstancedRobots("res/content/models/npc/npc23.fbx", glm::ivec2(5, 15),
                                                                                                                                                                                                                              &shaderRigInstanced2,
                                                                                                                                                                                                                              glm::vec3(4.0f, -3.0f, 0.0f), glm::vec3(250, 0, 400), glm::vec3(0.008f))),
                                                                                                                                                                                                      ir3(new InstancedRobots("res/content/models/npc/npc23.fbx", glm::ivec2(2, 10),
                                                                                                                                                                                                                              &shaderRigInstanced2,
                                                                                                                                                                                                                              glm::vec3(18.0f, 3.0f, 15.0f), glm::vec3(150, 0, 300), glm::vec3(0.008f))),
                                                                                                                                                                                                      ir4(new InstancedRobots("res/content/models/npc/npc23.fbx", glm::ivec2(2, 10),
                                                                                                                                                                                                                              &shaderRigInstanced,
                                                                                                                                                                                                                              glm::vec3(-19.0f, 3.0f, 15.0f), glm::vec3(150, 0, 300), glm::vec3(0.008f))),
                                                                                                                                                                                                      npcAnimation(new Animation("res/content/models/npc/npc23.fbx", ir)),
                                                                                                                                                                                                      npcAnimator(new Animator(npcAnimation, true)),
                                                                                                                                                                                                      npcAnimator2(new Animator(npcAnimation, true)),
                                                                                                                                                                                                      barmanAnimation(new Animation("res/content/models/barman_rignorig/BARMAN_ANIMATIONv2.fbx", barman)),
                                                                                                                                                                                                      barmanAnimator(new Animator(barmanAnimation, false)),
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
                                                                                                                                                                                                      pointLight6E(new Entity("pointLight6dj")),
                                                                                                                                                                                                      pointLight6(new PointLight()),
                                                                                                                                                                                                      pointLight7E(new Entity("pointLight7")),
                                                                                                                                                                                                      pointLight7(new PointLight()),
                                                                                                                                                                                                      pointLight8E(new Entity("pointLight8")),
                                                                                                                                                                                                      pointLight8(new PointLight()),
                                                                                                                                                                                                      fm(new ForwardMovement(pathToSong, glm::vec3(0, -2.5, 0), glm::vec3(0, -2.5, 47))),
                                                                                                                                                                                                      // hud
                                                                                                                                                                                                      player3(new Entity("player3")),
                                                                                                                                                                                                      resBar(new ResizableImage(&imageShaderGreen)),
                                                                                                                                                                                                      resBarEntity(new Entity("resBar")),
                                                                                                                                                                                                      lastTime(0.0),
                                                                                                                                                                                                      spawner(new Entity("Spawner")),
                                                                                                                                                                                                      spawnerComponent(new SpawnerComponent(pathToSong, glm::vec3(0, 0, orbDistance), 17)),
                                                                                                                                                                                                      canDecreaseBar(true),
                                                                                                                                                                                                      clock(5),
                                                                                                                                                                                                      fallStop(5),
                                                                                                                                                                                                      scoreEntity(new Entity("scoreEntity")),
                                                                                                                                                                                                      scoreImage(new Image(&imageShader)),
                                                                                                                                                                                                      scoreTexture(new Texture("res/content/textures/hud/score.png", "scoreTexture")),
                                                                                                                                                                                                      comboEntity(new Entity("comboEntity")),
                                                                                                                                                                                                      comboImage(new Image(&imageShader)),
                                                                                                                                                                                                      comboTexture(new Texture("res/content/textures/hud/combo.png", "comboTexture")),
                                                                                                                                                                                                      humanEntity(new Entity("humanEntity")),
                                                                                                                                                                                                      humanImage(new Image(&imageShader)),
                                                                                                                                                                                                      humanTexture(new Texture("res/content/textures/hud/glowa.png", "humanTexture")),
                                                                                                                                                                                                      robotEntity(new Entity("robotEntity")),
                                                                                                                                                                                                      robotImage(new Image(&imageShader)),
                                                                                                                                                                                                      robotTexture(new Texture("res/content/textures/hud/robot.png", "robotTexture")),
                                                                                                                                                                                                      frameEntity(new Entity("frameEntity")),
                                                                                                                                                                                                      frameImage(new Image(&imageShader)),
                                                                                                                                                                                                      frameTexture(new Texture("res/content/textures/hud/ramka.png", "frameTexture"))
    {
    }

    void changeActiveButton(Button *newActiveButton)
    {
        if (activeButton != nullptr)
        {
            activeButton->setActive(false);
        }

        activeButton = newActiveButton;

        if (activeButton != nullptr)
        {
            activeButton->setActive(true);
        }
        activeButton = newActiveButton;
    }

    void clickActiveButton()
    {
        if (activeButton != nullptr)
        {
            activeButton->onClick();
        }
    }

    void awake() override
    {
        // turn into static
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
        z = 5;
        ScoreNumbers::getInstance().setWidth(s.WINDOW_WIDTH);
        ScoreNumbers::getInstance().setHeight(s.WINDOW_HEIGHT);
    };

    void start() override
    {

        // audio
        // audioManager.init();

        // scene manager
        sm.setCurrentScene("MarcinScene");
        Scene2 *currentScene = sm.getSceneByName("MarcinScene");

        // Inicjalizacja spawnera
        // spawner = new Spawner(currentScene);
        spawner->addComponent(spawnerComponent);
        spawnerComponent->init();
        currentScene->addEntity(spawner);

        // screen shader
        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        // entities
        // club interior

        clubE->addComponent(club);
        currentScene->addEntity(clubE);
        club->getTransform()->rotate(glm::vec3(270.0f, 0.0f, 0.0f));
        club->getTransform()->setScale(glm::vec3(0.5f));
        club->getTransform()->setPosition(glm::vec3(0.0f, -3.4f, 0.0f));

        djE->addComponent(dj);
        currentScene->addEntity(djE);
        dj->getTransform()->setScale(glm::vec3(0.005f));
        dj->getTransform()->setPosition(glm::vec3(0.0f, -2.5f, 0.0f));

        scianyE->addComponent(sciany);
        currentScene->addEntity(scianyE);
        sciany->getTransform()->setScale(glm::vec3(0.5f));
        sciany->getTransform()->rotate(glm::vec3(270.0f, 0.0f, 0.0f));
        sciany->getTransform()->setPosition(glm::vec3(0.0f, -3.4f, 0.0f));

        barDrinksE->addComponent(barDrinks);
        currentScene->addEntity(barDrinksE);
        // barDrinks->getTransform()->rotate(glm::vec3(270.0f,0.0f, 0.0f));
        barDrinks->getTransform()->setScale(glm::vec3(0.005f));
        barDrinks->getTransform()->setPosition(glm::vec3(0.0f, -3.4f, 0.0f));

        canistersE->addComponent(canisters);
        currentScene->addEntity(canistersE);
        canisters->getTransform()->setScale(glm::vec3(0.5f));
        canisters->getTransform()->rotate(glm::vec3(270.0f, 0.0f, 0.0f));
        canisters->getTransform()->setPosition(glm::vec3(0.0f, -3.4f, 0.0f));

        chairsE->addComponent(chairs);
        currentScene->addEntity(chairsE);
        chairs->getTransform()->setScale(glm::vec3(0.005f));
        // chairs->getTransform()->rotate(glm::vec3(270.0f,0.0f,0.0f));
        chairs->getTransform()->setPosition(glm::vec3(0.0f, -3.4f, 0.0f));

        barmanE->addComponent(barman);
        currentScene->addEntity(barmanE);
        barman->getTransform()->setScale(glm::vec3(0.02f));
        barman->getTransform()->rotate(glm::vec3(0.0f, 180.0f, 0.0f));
        barman->getTransform()->setPosition(glm::vec3(0.0f, -3.4f, 50.728f));

        // lights
        pointLight->addComponent(pointLight1);
        pointLight->getTransform()->setScale(glm::vec3(5.f));
        currentScene->addEntity(pointLight);

        pointLight2E->addComponent(pointLight2);
        pointLight2E->getTransform()->setScale(glm::vec3(74.0f, 7.0f, 100.0f));
        pointLight2E->getTransform()->setPosition(glm::vec3(6.0f, -0.7f, 52.0f));
        currentScene->addEntity(pointLight2E);

        pointLight3E->addComponent(pointLight3);
        pointLight3E->getTransform()->setScale(glm::vec3(20.0f, 135.0f, 245.0f));
        pointLight3E->getTransform()->setPosition(glm::vec3(-5.0f, -0.7f, 52.0f));
        currentScene->addEntity(pointLight3E);

        pointLight4E->addComponent(pointLight4);
        pointLight4E->getTransform()->setScale(glm::vec3(74.0f, 7.0f, 100.0f));
        pointLight4E->getTransform()->setPosition(glm::vec3(-2.5f, 3.0f, 53.7f));
        currentScene->addEntity(pointLight4E);

        pointLight5E->addComponent(pointLight5);
        pointLight5E->getTransform()->setScale(glm::vec3(20.0f, 135.0f, 245.0f));
        pointLight5E->getTransform()->setPosition(glm::vec3(3.0f, 3.0f, 53.7f));
        currentScene->addEntity(pointLight5E);

        pointLight6E->addComponent(pointLight6);
        pointLight6E->getTransform()->setScale(glm::vec3(80.0f, 135.0f, 245.0f));
        pointLight6E->getTransform()->setPosition(glm::vec3(-0.27f, 8.33f, 54.1f));
        currentScene->addEntity(pointLight6E);

        pointLight7E->addComponent(pointLight7);
        pointLight7E->getTransform()->setScale(glm::vec3(20.0f, 135.0f, 245.0f));
        pointLight7E->getTransform()->setPosition(glm::vec3(3.0f, 3.0f, 3.7f));
        currentScene->addEntity(pointLight7E);

        pointLight8E->addComponent(pointLight8);
        pointLight8E->getTransform()->setScale(glm::vec3(80.0f, 135.0f, 245.0f));
        pointLight8E->getTransform()->setPosition(glm::vec3(0.0f, 3.0f, 0.0f));
        currentScene->addEntity(pointLight8E);

        currentScene->addEntity(sun);
        sun->addComponent(sunLight);

        currentScene->addEntity(dancingRobots);
        dancingRobots->addComponent(ir);

        currentScene->addEntity(dancingRobots2);
        dancingRobots->addComponent(ir2);

        currentScene->addEntity(dancingRobots3);
        dancingRobots->addComponent(ir3);

        currentScene->addEntity(dancingRobots4);
        dancingRobots->addComponent(ir4);

        currentScene->addEntity(sphere1);
        sphere1->addComponent(sphere);
        sphere->getTransform()->setPosition(lightPos);

        // movement
        currentScene->addEntity(fm);
        fm->getTransform()->setPosition(glm::vec3(0, -2.5, 0));

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

        // hud
        currentScene->addEntity(resBarEntity);
        resBarEntity->addComponent(resBar);
        resBar->getTransform()->setScale(glm::vec3(0.2f, 0.041f, 0.0f));
        resBar->getTransform()->setPosition(glm::vec3(-0.699f, -0.699f, 0.0f));

        // txt
        // comboRenderer->setParameters("Combo " + std::to_string(combo) + "x", 150, 950, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);
        // scoreRenderer->setParameters("Score " + std::to_string(score), 1920 / 2 - 12, 950, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);
        // scoreRenderer->setParameters("Score " + std::to_string(score), 768, 601, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);

        //hud2
        currentScene->addEntity(scoreEntity);
        scoreEntity->addComponent(scoreImage);
        scoreImage->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.0f));
        scoreImage->getTransform()->setPosition(glm::vec3(-0.85f, 0.75f, 0.0f));
        scoreImage->setTexture(scoreTexture);
        currentScene->addEntity(comboEntity);
        comboEntity->addComponent(comboImage);
        comboImage->getTransform()->setScale(glm::vec3(0.16f, 0.24f, 0.0f));
        comboImage->getTransform()->setPosition(glm::vec3(0.5f, -0.23f, 0.0f));
        comboImage->setTexture(comboTexture);
        currentScene->addEntity(humanEntity);
        humanEntity->addComponent(humanImage);
        humanImage->getTransform()->setScale(glm::vec3(0.024f, 0.04f, 0.0f));
        humanImage->getTransform()->setPosition(glm::vec3(-0.942f, -0.7f, 0.0f));
        humanImage->setTexture(humanTexture);
        currentScene->addEntity(robotEntity);
        robotEntity->addComponent(robotImage);
        robotImage->getTransform()->setScale(glm::vec3(0.018f, 0.05f, 0.0f));
        robotImage->getTransform()->setPosition(glm::vec3(-0.46f, -0.7f, 0.0f));
        robotImage->setTexture(robotTexture);
        currentScene->addEntity(frameEntity);
        frameEntity->addComponent(frameImage);
        frameImage->getTransform()->setScale(glm::vec3(0.204f, 0.05f, 0.0f));
        frameImage->getTransform()->setPosition(glm::vec3(-0.699f, -0.7f, 0.0f));
        frameImage->setTexture(frameTexture);

        DrunkShader.setInt("screenTexture", 0);
    };

    void update() override
    {

        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);

        time = time + s.deltaTime;

        deltaTime = s.deltaTime;
        //        AudioManager::getInstance().playThisSong("bicik");
        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        barmanAnimator->UpdateAnimation(deltaTime, 90.0f);
        djAnimator->UpdateAnimation(deltaTime, 90.0f);

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();
        ScoreNumbers::getInstance().setProjection(projection);
        ScoreNumbers::getInstance().setView(view);
        //        s.camera.MoveForwardZ(globalVelocity * deltaTime);

        // moving forward
        if (deltaTime < 1)
        {
            player->getTransform()->translate(glm::vec3(0.0f, 0.0f, deltaTime * globalVelocity));
            z += deltaTime * globalVelocity;
            s.camera.MoveForwardZ(globalVelocity * deltaTime);
            pointLight8E->getTransform()->translate(glm::vec3(0.0f, 0.0f, deltaTime * globalVelocity));
        }

        npcAnimator->UpdateAnimation(s.deltaTime, lookatAngle);
        shaderRigInstanced.use();
        auto transforms2 = npcAnimator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms2.size(); ++i)
            shaderRigInstanced.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms2[i]);

        npcAnimator2->UpdateAnimation(s.deltaTime, lookatAngle * (-1.0f));
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
            shaderBarmanRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms4[i]);

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
        // scene.update();

        joystickOffset = playerInput.getJoystick(2);
        joystickOffset2 = playerInput.getJoystick(1);
        joystickOffset3 = playerInput1.getJoystick(2);
        joystickOffset4 = playerInput1.getJoystick(1);

        if (timer < 0)
        {
            timer = effectTime;
            currentDrink = DrinkType::None;
        }

        buffer.unbind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        switch (currentDrink)
        {
        case DrinkType::Drunk:
            DrunkShader.use();
            DrunkShader.setFloat("time", time);
            DrunkShader.setInt("screenTexture", 0);
            timer -= s.deltaTime;
            clock -= s.deltaTime;
            canDecreaseBar = false;

            break;
        case DrinkType::InverseInput:
            shaderNoneDrink.use();
            shaderNoneDrink.setInt("screenTexture", 0);

            joystickOffset = -joystickOffset;
            joystickOffset2 = -joystickOffset2;
            joystickOffset3 = -joystickOffset3;
            joystickOffset4 = -joystickOffset4;
            timer -= s.deltaTime;
            clock -= s.deltaTime;
            canDecreaseBar = false;

            break;
        case DrinkType::UpsideDown:

            reverseShader.use();
            reverseShader.setFloat("time", time);
            reverseShader.setInt("screenTexture", 0);
            timer -= s.deltaTime;
            clock -= s.deltaTime;
            canDecreaseBar = false;

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
        // imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // hud
        //hud2
        imageShader.use();
        scoreImage->renderPlane();
        comboImage->renderPlane();
        humanImage->renderPlane();
        robotImage->renderPlane();
        frameImage->renderPlane();

        resBar->renderPlane();
        // resizing bar
        // temporary------------------------------------------------------------------------------------
        double currentTime = glfwGetTime();
        // Jeśli upłynęła 1 sekunda od ostatniej aktualizacji
        if (currentTime - lastUpdateTime >= resizeInterval)
        {
            if (time < songLenghtGlobal && canDecreaseBar)
                resBar->resizeOnImpulse(resizeAmount);
            lastUpdateTime = currentTime;
            if (lookatAngle < 170.0f)
            {
                lookatAngle += 5.0f;
            }
        }
        // Jeśli score został zwiększony o incrementScore
        if (score - lastScore >= incrementScore)
        {
            resBar->increaseOnImpulse(resizeAmount);
            lastScore = score;

            if (lookatAngle > 5.0f)
            {
                lookatAngle -= 5.0f;
            }
        }
        // resBar może nowu spadać
        if (!canDecreaseBar && timer < 0)
        {
            canDecreaseBar = true;
            // lock = fallStop; // resetujemy timer
        }
        // zmiana koloru paska na szary gdy nie mozę spadać
        imageShaderGreen.use();
        if (canDecreaseBar)
        {
            imageShaderGreen.setBool("isBarLocked", false);
        }
        else
        {
            imageShaderGreen.setBool("isBarLocked", true);
        }
        // std::cout<<resBar->getTransform()->getLocalScale().x<<std::endl;
        // giving a 2 second chance to player to bumpup the bar
        if (resBar->getTransform()->getLocalScale().x <= 0.01f)
        {
            if (!isCounting)
            {
                isCounting = true; // Rozpocznij odliczanie
            }
        }
        else
        {
            isCounting = false; // Zatrzymaj odliczanie i zresetuj czas
            timeLeft = 2.0f;
        }

        if (isCounting)
        {
            timeLeft -= s.deltaTime;
            if (timeLeft <= 0.0f)
            {
                //sm.setCurrentScene("LoseScene");
            }
        }

        // temporary------------------------------------------------------------------------------------
        // text
        comboRenderer->setParameters("x" + std::to_string(combo), 1920 / 2 + 470, 370, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);
        scoreRenderer->setParameters(std::to_string(score), 200, 865, 1.2f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);

        comboRenderer->renderText();
        scoreRenderer->renderText();
        ScoreNumbers::getInstance().update();
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

        if (playerInput.isKeyPressed(1))
        {
            sm.setCurrentScene("PauseScene");
        }

        if (time > songLenghtGlobal + 5)
        {
            sm.setCurrentScene("WinScene");
        }
    };

    void onExit()
    {
        AudioManager::getInstance().pauseThisSong("bicik");
    };

    void onDestroy() override {

    };

    ~exampleSceneScript() override = default;
};

#endif
