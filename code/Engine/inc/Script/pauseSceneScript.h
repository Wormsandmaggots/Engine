#ifndef ENGINE_PAUSESCENESCRIPT_H
#define ENGINE_PAUSESCENESCRIPT_H

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

class pauseSceneScript : public SceneScript {
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
    Image* pauseWallpaper;

    Button* continueButton;
    Button* changeSongButton;
    Button* exitButton;

    Entity* pauseBackground;
    Entity* cn;
    Entity* cs;
    Entity* ex;

    Texture* background;
    Texture* cn_button_idle;
    Texture* cs_button_idle;
    Texture* ex_button_idle;
    Texture* cn_button_activ;
    Texture* cs_button_activ;
    Texture* ex_button_activ;

    Button* activeButton;

    float lastButtonChangeTime;
    float buttonChangeDelay;
    bool joystickReset;

public:
    // Konstruktor domyślny
    pauseSceneScript(EditorLayer::Editor& editor, CollisionManager& cm, SceneManager& sm, SSAO& ssao, Renderer& renderer, AudioManager& audioManager, PlayerInput& playerInput,
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
            pauseWallpaper(new Image(&imageShader)),
            continueButton(new Button(&imageShader)),
            changeSongButton(new Button(&imageShader)),
            exitButton(new Button(&imageShader)),

            pauseBackground(new Entity("mainMenu")),
            cn(new Entity("continueButton")),
            cs(new Entity("changeSongButton")),
            ex(new Entity("exitButton")),

            background(new Texture("res/content/textures/pause/pause_background.png", "background")),
            cn_button_idle(new Texture("res/content/textures/pause/continue_d.png", "continue_d")),
            cs_button_idle(new Texture("res/content/textures/pause/change_d.png", "change_d")),
            ex_button_idle(new Texture("res/content/textures/pause/exit_d.png", "exit_d")),
            cn_button_activ(new Texture("res/content/textures/pause/continue_h.png", "continue_h")),
            cs_button_activ(new Texture("res/content/textures/pause/change_h.png", "change_h")),
            ex_button_activ(new Texture("res/content/textures/pause/exit_h.png", "exit_h"))

    {
    }

    // Dodajemy metodę do zmiany aktywnego przycisku
    void changeActiveButton(Button* newActiveButton) {
        if (activeButton != nullptr) {
            activeButton->setActive(false);
            if(activeButton == continueButton){
                activeButton->getTransform()->setScale(glm::vec3(0.16f,0.04f,0.2f));
            }
            else if(activeButton == changeSongButton){
                activeButton->getTransform()->setScale(glm::vec3(0.22f, 0.043f, 0.2f));
            }
        }

        activeButton = newActiveButton;

        if (activeButton != nullptr) {
            activeButton->setActive(true);
            if(activeButton == continueButton){
                activeButton->getTransform()->setScale(glm::vec3(0.18f,0.06f,0.2f));
            }
            else if(activeButton == changeSongButton){
                activeButton->getTransform()->setScale(glm::vec3(0.235f, 0.059f, 0.214f));
            }
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
        sm.setCurrentScene("PauseScene");
        
        Scene2* currentScene = sm.getSceneByName("PauseScene");

        //main menu

        //menu background
        currentScene->addEntity(pauseBackground);
        pauseBackground->addComponent(pauseWallpaper);
        pauseWallpaper->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        pauseWallpaper->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        pauseWallpaper->setTexture(background);

        //new game
        currentScene->addEntity(cn);
        cn->addComponent(continueButton);
        continueButton->getTransform()->setScale(glm::vec3(0.18f,0.06f,0.2f));
        continueButton->getTransform()->setPosition(glm::vec3(0.0f, -0.031f, 0.0f));

        //this button will be activ from start, so we set it's texture as activ form the begining
        continueButton->setTexture(cn_button_activ);
        continueButton->setInactiveTexture(cn_button_idle);
        continueButton->setActiveTexture(cn_button_activ);

        continueButton->setOnClick([this]() {
            std::cout << "Start button clicked!" << std::endl;
            this->sm.setCurrentScene("MarcinScene");
        });

        //exit
        currentScene->addEntity(cs);
        cs->addComponent(changeSongButton);
        changeSongButton->getTransform()->setScale(glm::vec3(0.22f, 0.043f, 0.2f));
        changeSongButton->getTransform()->setPosition(glm::vec3(0.0f, -0.25f, 0.0f));

        changeSongButton->setTexture(cs_button_idle);
        changeSongButton->setInactiveTexture(cs_button_idle);
        changeSongButton->setActiveTexture(cs_button_activ);

        changeSongButton->setOnClick([this]() {
            std::cout << "changeButton button clicked!" << std::endl;
            this->sm.setCurrentScene("SongScene");
        });

        //credits
        currentScene->addEntity(ex);
        ex->addComponent(exitButton);
        exitButton->getTransform()->setScale(glm::vec3(0.17f, 0.047f, 0.2f));
        exitButton->getTransform()->setPosition(glm::vec3(0.0f, -0.488f, 0.0f));

        exitButton->setTexture(ex_button_idle);
        exitButton->setInactiveTexture(ex_button_idle);
        exitButton->setActiveTexture(ex_button_activ);

        exitButton->setOnClick([]() {
            std::cout << "Exit button clicked!" << std::endl;
            glfwSetWindowShouldClose(s.window, GL_TRUE);
        });

        //buttons on scene handling
        activeButton = continueButton;
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
        pauseWallpaper->renderPlane();

        continueButton->renderPlane();
        changeSongButton->renderPlane();
        exitButton->renderPlane();


        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        if ((isDelayPassed && isJoystickMoved) || (joystickReset && isJoystickMoved)) {
            if (joystickOffset.y < 0.5) {
                if (activeButton == continueButton) {
                    changeActiveButton(exitButton);
                } else if (activeButton == changeSongButton) {
                    changeActiveButton(continueButton);
                } else if (activeButton == exitButton) {
                    changeActiveButton(changeSongButton);
                }
            }
            else if (joystickOffset.y > -0.5) {
                if (activeButton == continueButton) {
                    changeActiveButton(changeSongButton);
                } else if (activeButton == changeSongButton) {
                    changeActiveButton(exitButton);
                } else if (activeButton == exitButton) {
                    changeActiveButton(continueButton);
                }
            }
            lastButtonChangeTime = currentFrame;
            joystickReset = false;
        } else if (std::abs(joystickOffset.y) <= 0.5) {
            joystickReset = true;
        }

        if (playerInput.isKeyPressed(0) || playerInput.isKeyPressed(1)) {
            clickActiveButton();
        }

    };

    void onDestroy() override{
    };

    ~pauseSceneScript() override = default;
};


#endif
