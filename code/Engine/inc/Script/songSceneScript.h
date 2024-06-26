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
    EditorLayer::Editor& editor;
    // collision
    CollisionManager& cm;
    // scene manager
    SceneManager& sm;
    // audio
    AudioManager& audioManager;

    float songSampleInterval;
    std::vector<SongSample> songData;
    int songDataIndex;

    // input joystick
    int connectedControllers;
    PlayerInput& playerInput;
    PlayerInput& playerInput1;
    glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;
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
    FrameBuffer buffer;

    // ssao
    SSAO ssao;
    // renderer
    Renderer& renderer;
    // model

    //HUD
    //main menu
    Image* manuBackground;

    Button* era00;
    Button* era80;
    Button* era90;

    Button* backToMenu;
    Button* startGame;

    Entity* menuWalpaper;
    Entity* ng;
    Entity* ex;
    Entity* cr;

    Entity* bm;
    Entity* sg;

    Texture* background;
    Texture* button00_idle;
    Texture* button80_idle;
    Texture* button90_idle;
    Texture* button00_activ;
    Texture* button80_activ;
    Texture* button90_activ;

    Texture* buttonmenu_idle;
    Texture* buttonmenu_activ;
    Texture* buttonstart_idle;
    Texture* buttonstart_activ;

    Button* activeButton;

    float lastButtonChangeTime;
    float buttonChangeDelay;
    bool joystickReset;

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
            buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),
            //hud
            manuBackground(new Image(&imageShader)),
            era00(new Button(&imageShader)),
            era80(new Button(&imageShader)),
            era90(new Button(&imageShader)),
            startGame(new Button(&imageShader)),
            backToMenu(new Button(&imageShader)),


            menuWalpaper(new Entity("mainMenu")),
            ng(new Entity("era00")),
            ex(new Entity("era80")),
            cr(new Entity("era90")),
            sg(new Entity("startGame")),
            bm(new Entity("backToMenu")),

//            background(new Texture("res/content/textures/background.png", "background")),
//            button00_idle(new Texture("ares/content/textures/britney.png", "start_d")),
//            button80_idle(new Texture("ares/content/textures/john.png", "exit_d")),
//            button90_idle(new Texture("ares/content/textures/queen.png", "credits_d")),
//            button00_activ(new Texture("res/content/textures/britney.png", "start_h")),
//            button80_activ(new Texture("res/content/textures/john.png", "exit_h")),
//            button90_activ(new Texture("res/content/textures/queen.png", "credits_h"))
            background(new Texture("res/content/textures/songscene_b.png", "background")),
            button00_idle(new Texture("res/content/textures/songScene/00_d.png", "start_d")),
            button90_idle(new Texture("res/content/textures/songScene/90_d.png", "exit_d")),
            button80_idle(new Texture("res/content/textures/songScene/80_d.png", "credits_d")),
            button00_activ(new Texture("res/content/textures/songScene/00_h.png", "start_h")),
            button90_activ(new Texture("res/content/textures/songScene/90_h.png", "exit_h")),
            button80_activ(new Texture("res/content/textures/songScene/80_h.png", "credits_h")),
            buttonmenu_idle(new Texture("res/content/textures/songScene/mainmenu_d.png", "menu_d")),
            buttonstart_idle(new Texture("res/content/textures/songScene/letsdance_d.png", "letsdance_d")),
            buttonmenu_activ(new Texture("res/content/textures/songScene/mainmenu_h.png", "menu_h")),
            buttonstart_activ(new Texture("res/content/textures/songScene/letsdance_h.png", "letsdance_h"))
    {
    }

    // Dodajemy metodę do zmiany aktywnego przycisku
    void changeActiveButton(Button* newActiveButton) {
        if (activeButton != nullptr) {
            activeButton->setActive(false);
            activeButton->getTransform()->setScale(glm::vec3(0.23f, 0.057f, 0.23f));
        }

        activeButton = newActiveButton;

        if (activeButton != nullptr) {
            activeButton->setActive(true);
            activeButton->getTransform()->setScale(glm::vec3(0.25f, 0.09f, 0.25f));
        }
        activeButton = newActiveButton;
    }

    // Dodajemy metodę do wywołania funkcji onClick dla aktywnego przycisku
    void clickActiveButton() {
        if (activeButton != nullptr) {
            activeButton->onClick();
        }
    }

    void awake() override{
    };

    void start() override{
        //scene manager
        sm.setCurrentScene("SongScene");
        Scene2* currentScene = sm.getSceneByName("SongScene");
        //main menu

        //menu background
        currentScene->addEntity(menuWalpaper);
        menuWalpaper->addComponent(manuBackground);
        manuBackground->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        manuBackground->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        manuBackground->setTexture(background);

        //new game
        currentScene->addEntity(ng);
        ng->addComponent(era00);
        era00->getTransform()->setScale(glm::vec3(0.25f, 0.09f, 0.25f));
        era00->getTransform()->setPosition(glm::vec3(0.0f, -0.30f, 0.0f));

        //this button will be activ from start, so we set it's texture as activ form the begining
        era00->setTexture(button00_activ);
        era00->setInactiveTexture(button00_idle);
        era00->setActiveTexture(button00_activ);

        era00->setOnClick([this]() {
            std::cout << "00 button clicked!" << std::endl;
            pathToSong = "res/content/sounds/songs/00/dream.wav";
//            pathToSong = "res/content/sounds/effects/kicked out.wav";
            //this->sm.setCurrentScene("CalibrationScene");
            this->sm.setCurrentScene("MarcinScene");
        });

        //credits
        currentScene->addEntity(cr);
        cr->addComponent(era90);
        era90->getTransform()->setScale(glm::vec3(0.23f, 0.057f, 0.23f));
        era90->getTransform()->setPosition(glm::vec3(0.0f, -0.50f, 0.0f));

        era90->setTexture(button90_idle);
        era90->setInactiveTexture(button90_idle);
        era90->setActiveTexture(button90_activ);

        era90->setOnClick([this]() {
            std::cout << "90 button clicked!" << std::endl;
            pathToSong = "res/content/sounds/songs/90/eurodance.wav";
            //this->sm.setCurrentScene("CalibrationScene");
            this->sm.setCurrentScene("MarcinScene");
        });

        //exit
        currentScene->addEntity(ex);
        ex->addComponent(era80);
        era80->getTransform()->setScale(glm::vec3(0.23f, 0.057f, 0.23f));
        era80->getTransform()->setPosition(glm::vec3(0.0f, -0.70f, 0.0f));

        era80->setTexture(button80_idle);
        era80->setInactiveTexture(button80_idle);
        era80->setActiveTexture(button80_activ);

        era80->setOnClick([this]() {
            std::cout << "80 button clicked!" << std::endl;
            pathToSong = "res/content/sounds/songs/80/goddess.wav";
            //this->sm.setCurrentScene("CalibrationScene");
            this->sm.setCurrentScene("MarcinScene");
        });

        //mainmenu
        currentScene->addEntity(bm);
        bm->addComponent(backToMenu);
        backToMenu->getTransform()->setScale(glm::vec3(0.23f, 0.057f, 0.23f));
        backToMenu->getTransform()->setPosition(glm::vec3(-0.628f, -0.828f, 0.0f));

        backToMenu->setTexture(buttonmenu_idle);
        backToMenu->setInactiveTexture(buttonmenu_idle);
        backToMenu->setActiveTexture(buttonmenu_activ);

        backToMenu->setOnClick([this]() {
            std::cout << "backToMenu button clicked!" << std::endl;
            //pathToSong = "res/content/sounds/songs/80/goddess.wav";
            //this->sm.setCurrentScene("CalibrationScene");
            this->sm.setCurrentScene("KubaScene");
        });

        //letsdance
        currentScene->addEntity(sg);
        sg->addComponent(startGame);
        startGame->getTransform()->setScale(glm::vec3(0.23f, 0.057f, 0.23f));
        startGame->getTransform()->setPosition(glm::vec3(0.611f, -0.828f, 0.0f));

        startGame->setTexture(buttonstart_idle);
        startGame->setInactiveTexture(buttonstart_idle);
        startGame->setActiveTexture(buttonstart_activ);

        startGame->setOnClick([this]() {
            std::cout << "startgame button clicked!" << std::endl;
            //pathToSong = "res/content/sounds/songs/80/goddess.wav";
            //this->sm.setCurrentScene("CalibrationScene");
            this->sm.setCurrentScene("MarcinScene");
        });

        //buttons on scene handling
        activeButton = era00;
        lastButtonChangeTime = 0.0f;
        buttonChangeDelay = 0.2f;
        joystickReset = true;
    };

    void update() override{
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);

        //time = time + s.deltaTime;

        deltaTime = s.deltaTime;

        bool isDelayPassed = currentFrame - lastButtonChangeTime >= buttonChangeDelay;
        bool isJoystickMoved = std::abs(joystickOffset.y) > 0.5;

        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();

        joystickOffset = playerInput.getJoystick(2);
        joystickOffset2 = playerInput.getJoystick(1);
        joystickOffset3 = playerInput1.getJoystick(2);
        joystickOffset4 = playerInput1.getJoystick(1);

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        renderer.updateProjectionAndView(projection, view, s.camera.Position);

        editor.draw();
        sm.updateLoadedScenes();

        glDisable(GL_DEPTH_TEST);
        //imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //hud
        glActiveTexture(GL_TEXTURE0);
        imageShader.use();
        manuBackground->renderPlane();

        era00->renderPlane();
        era80->renderPlane();
        era90->renderPlane();
        startGame->renderPlane();
        backToMenu->renderPlane();


        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        if ((isDelayPassed && isJoystickMoved) || (joystickReset && isJoystickMoved)) {
            LOG_INFO(std::to_string(joystickOffset.x));
            if (joystickOffset.y < 0.5) {
                if (activeButton == era00) {
                    changeActiveButton(era80);
                } else if (activeButton == era90) {
                    changeActiveButton(era00);
                } else if (activeButton == era80) {
                    changeActiveButton(era90);
                }
            }
            else if (joystickOffset.y > -0.5) {
                if (activeButton == era00) {
                    changeActiveButton(era90);
                } else if (activeButton == era80) {
                    changeActiveButton(era00);
                } else if (activeButton == era90) {
                    changeActiveButton(era80);
                }
            }
            else if (joystickOffset.x > -0.5){
                changeActiveButton(backToMenu);
            }
            else if (joystickOffset.x < 0.5){
                changeActiveButton(startGame);
            }
            lastButtonChangeTime = currentFrame;
            joystickReset = false;
        } else if (std::abs(joystickOffset.y) <= 0.5) {
            joystickReset = true;
        }

        if (playerInput.isKeyPressed(1)) {
            clickActiveButton();
        }

    };

    void onDestroy() override{
    };

    ~songSceneScript() override = default;
};


#endif
