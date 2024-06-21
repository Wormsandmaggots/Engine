#ifndef ENGINE_MENUSCENESCRIPT_H
#define ENGINE_MENUSCENESCRIPT_H

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

#include "Script/exampleSceneScript.h"
#include "HUD/Button.h"
#include "HUD/ResizableImage.h"
#include "HUD/Image.h"
#include "Globals.h"

#include "Script/SceneScript.h"




class menuSceneScript: public SceneScript{
private:

    EditorLayer::Editor& editor;
    // collision
    CollisionManager& cm;
    // scene manager
    SceneManager& sm;

    Renderer renderer;

    // audio
    AudioManager& audioManager;

    float songSampleInterval;
    std::vector<SongSample> songData;
    int songDataIndex;

    // input joystick
    int connectedControllers;
    PlayerInput& playerInput;
    PlayerInput& playerInput1;

    /*glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;*/

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

    // Deklaracja obiektów jako pól klasy
    Image* menuBackground;
    Image* menuColumn;
    Button* newGame;
    Button* exit;

    //for resizing bar
    double lastTime = 0.0;

public:

    menuSceneScript(EditorLayer::Editor& editor, CollisionManager& cm, SceneManager& sm, AudioManager& audioManager, PlayerInput& playerInput,
    PlayerInput& playerInput1, DebugInput& debugInput, Shader& shader, Shader& collisionTestShader, Shader& shaderText,
            Shader& colorShader, Shader& shaderPbr, Shader& screenShader, Shader& shaderRig, Shader& shaderBarmanRig, Shader& DrunkShader,
            Shader& shaderNoneDrink, Shader& reverseShader, Shader& imageShader, Shader& imageShaderGreen, Shader& shaderRigInstanced) :
    editor(editor),
    cm(cm),
    sm(sm),
    audioManager(audioManager),
    songSampleInterval(1.0),
    songDataIndex(0),
    connectedControllers(JslConnectDevices()),
    playerInput(playerInput),
    playerInput1(playerInput1),
    debugInput(debugInput),
    /*joystickOffset(glm::vec2(0)),
    joystickOffset2(glm::vec2(0)),
    joystickOffset3(glm::vec2(0)),
    joystickOffset4(glm::vec2(0)),*/
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
    renderer(&shader),
    lastTime(0.0)
    {
    }

    void awake() override{};
    void start() override{
        //textures
        Texture menuBack("res/content/textures/backgg.jpg", "backgg");
        Texture menuVerticalColumn("res/content/textures/pole.png", "pole");
        Texture menuNewGame("res/content/textures/ng.png", "ng");
        Texture menuExit("res/content/textures/ex.png", "ex");
        Texture menuNewGameAlt("res/content/textures/nodes.png", "nodes");

        //Objects
        menuBackground = new Image(&imageShader);
        menuBackground->setTexture(&menuBack);

        menuColumn = new Image(&imageShader);
        menuColumn->setTexture(&menuVerticalColumn);

        newGame = new Button(&imageShader);
        newGame->setTexture(&menuNewGame);
        /*newGame->setOnClick([]() {
            std::cout << "start game" << std::endl;
        });*/

        newGame->setOnClick([this]() {
            std::cout << "start game" << std::endl;
            if (sm.getCurrentScene()->getName() == "MarcinScene") {
                sm.setCurrentScene("exampleScene");
            } else {
                //sm->setCurrentScene("MarcinScene");
            }
        });

        exit = new Button(&imageShader);
        exit->setTexture(&menuExit);
        exit->setOnClick([this]() {
            std::cout << "wyjscie" << std::endl;
            glfwSetWindowShouldClose(s.window, true);
        });

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

        // Dodaj tekstury do wektora
        //std::vector<Texture*> animationTextures = {&menuNewGame, &menuNewGameAlt};

        // Ustaw tekstury animacji dla przycisku
        //newGame->setAnimationTextures(animationTextures);

    };

    void update() override{

        glDisable(GL_DEPTH_TEST);
        imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

        //sm->updateLoadedScenes();

        //TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////
        double mouseX, mouseY;
        glfwGetCursorPos(s.window, &mouseX, &mouseY);

        newGame->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        newGame->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

        exit->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        exit->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        //TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////

    };

    void onDestroy() override{
        // Pamiętaj o zwolnieniu pamięci
        delete menuBackground;
        delete menuColumn;
        delete newGame;
        delete exit;
    };
    ~menuSceneScript() override = default;
};

#endif
