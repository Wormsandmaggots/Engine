#ifndef ENGINE_MAINSCENESCRIPT_H
#define ENGINE_MAINSCENESCRIPT_H

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

class menuSceneScript : public SceneScript {
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
    SSAO ssao;
///////////////////////////////////////////////////
    // renderer
    Renderer& renderer;
///////////////////////////////////////////////////
    // model

    //HUD
    //main menu
    Image* manuBackground;

    Button* startButton;
    Button* exitButton;
    Button* creditsButton;

    Entity* menuWalpaper;
    Entity* ng;
    Entity* ex;
    Entity* cr;

    Texture* background;
    Texture* ng_button_idle;
    Texture* ex_button_idle;
    Texture* cr_button_idle;
    Texture* ng_button_activ;
    Texture* ex_button_activ;
    Texture* cr_button_activ;

    Button* activeButton;

    float lastButtonChangeTime;
    float buttonChangeDelay;
    bool joystickReset;

public:
    // Konstruktor domyślny
    menuSceneScript() :
            songSampleInterval(1.0),
            songDataIndex(0),
            connectedControllers(JslConnectDevices()),
            joystickOffset(glm::vec2(0)),
            joystickOffset2(glm::vec2(0)),
            joystickOffset3(glm::vec2(0)),
            joystickOffset4(glm::vec2(0)),
            buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT)),
            //hud
            manuBackground(new Image(&imageShader)),
            startButton(new Button(&imageShader)),
            exitButton(new Button(&imageShader)),
            creditsButton(new Button(&imageShader)),

            menuWalpaper(new Entity("mainMenu")),
            ng(new Entity("startButton")),
            ex(new Entity("exitButton")),
            cr(new Entity("creditsButton")),

            background(new Texture("res/content/textures/background.png", "background")),
            ng_button_idle(new Texture("res/content/textures/start_d.png", "start_d")),
            ex_button_idle(new Texture("res/content/textures/exit_d.png", "exit_d")),
            cr_button_idle(new Texture("res/content/textures/credits_d.png", "credits_d")),
            ng_button_activ(new Texture("res/content/textures/start_h.png", "start_h")),
            ex_button_activ(new Texture("res/content/textures/exit_h.png", "exit_h")),
            cr_button_activ(new Texture("res/content/textures/credits_h.png", "credits_h"))
    {
    }

    // Dodajemy metodę do zmiany aktywnego przycisku
    void changeActiveButton(Button* newActiveButton) {
        if (activeButton != nullptr) {
            activeButton->setActive(false);
        }

        activeButton = newActiveButton;

        if (activeButton != nullptr) {
            activeButton->setActive(true);
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
        sm.setCurrentScene("KubaScene");

        //main menu

        //menu background
        sm.getSceneByName("KubaScene")->addEntity(menuWalpaper);
        menuWalpaper->addComponent(manuBackground);
        manuBackground->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        manuBackground->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        manuBackground->setTexture(background);

        //new game
        sm.getSceneByName("KubaScene")->addEntity(ng);
        ng->addComponent(startButton);
        startButton->getTransform()->setScale(glm::vec3(0.14f, 0.06f, 0.2f));
        startButton->getTransform()->setPosition(glm::vec3(-0.75f, -0.031f, 0.0f));

        //this button will be activ from start, so we set it's texture as activ form the begining
        startButton->setTexture(ng_button_activ);
        startButton->setInactiveTexture(ng_button_idle);
        startButton->setActiveTexture(ng_button_activ);

        startButton->setOnClick([this]() {
            std::cout << "Start button clicked!" << std::endl;
            this->sm.setCurrentScene("MarcinScene");
        });

        //exit
        sm.getSceneByName("KubaScene")->addEntity(ex);
        ex->addComponent(exitButton);
        exitButton->getTransform()->setScale(glm::vec3(0.1f, 0.07f, 0.2f));
        exitButton->getTransform()->setPosition(glm::vec3(-0.75f, -0.25f, 0.0f));

        exitButton->setTexture(ex_button_idle);
        exitButton->setInactiveTexture(ex_button_idle);
        exitButton->setActiveTexture(ex_button_activ);

        exitButton->setOnClick([]() {
            std::cout << "Exit button clicked!" << std::endl;
            glfwSetWindowShouldClose(s.window, GL_TRUE);
        });

        //credits
        sm.getSceneByName("KubaScene")->addEntity(cr);
        cr->addComponent(creditsButton);
        creditsButton->getTransform()->setScale(glm::vec3(0.14f, 0.07f, 0.2f));
        creditsButton->getTransform()->setPosition(glm::vec3(-0.75f, -0.488f, 0.0f));

        creditsButton->setTexture(cr_button_idle);
        creditsButton->setInactiveTexture(cr_button_idle);
        creditsButton->setActiveTexture(cr_button_activ);

        creditsButton->setOnClick([]() {
            std::cout << "Credits button clicked!" << std::endl;
        });

        //buttons on scene handling
        activeButton = startButton;
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

        startButton->renderPlane();
        exitButton->renderPlane();
        creditsButton->renderPlane();


        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        if ((isDelayPassed && isJoystickMoved) || (joystickReset && isJoystickMoved)) {
            if (joystickOffset.y < 0.5) {
                if (activeButton == startButton) {
                    changeActiveButton(creditsButton);
                } else if (activeButton == exitButton) {
                    changeActiveButton(startButton);
                } else if (activeButton == creditsButton) {
                    changeActiveButton(exitButton);
                }
            }
            else if (joystickOffset.y > -0.5) {
                if (activeButton == startButton) {
                    changeActiveButton(exitButton);
                } else if (activeButton == exitButton) {
                    changeActiveButton(creditsButton);
                } else if (activeButton == creditsButton) {
                    changeActiveButton(startButton);
                }
            }
            lastButtonChangeTime = currentFrame;
            joystickReset = false;
        } else if (std::abs(joystickOffset.y) <= 0.5) {
            joystickReset = true;
        }

//debugging
//        if (activeButton == startButton) {
//            std::cout << "Start button is active" << std::endl;
//        } else if (activeButton == exitButton) {
//            std::cout << "Exit button is active" << std::endl;
//        } else if (activeButton == creditsButton) {
//            std::cout << "Credits button is active" << std::endl;
//        }

        if (playerInput.isKeyPressed(1)) {
            clickActiveButton();
            //debugging
            //std::cout<<"B"<<std::endl;
        }

    };

    void onDestroy() override{
    };

    ~menuSceneScript() override = default;
};


#endif
