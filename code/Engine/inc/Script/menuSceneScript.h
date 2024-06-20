#ifndef ENGINE_MENUSCENESCRIPT_H
#define ENGINE_MENUSCENESCRIPT_H

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
#include "Script/BaseSceneScript.h"

using namespace SceneManagement;

class menuSceneScript : public SceneScript {
private:

//    std::shared_ptr<EditorLayer::Editor>* editor;
//    std::shared_ptr<AudioManager>* audioManager;
//    std::shared_ptr<SceneManager>* sm;
    EditorLayer::Editor* editor;
    AudioManager* audioManager;
    SceneManager* sm;

    PlayerInput* playerInput;
    PlayerInput* playerInput1;
    DebugInput* debugInput;
    Shader* shader;
    Shader* shaderText;
    Shader* shaderPbr;
    Shader* screenShader;
    Shader imageShader;
    Shader* imageShaderGreen;
    Renderer* renderer;
    double lastTime;

    //main menu
    Image* mainMenu;

    Button* startButton;
    Button* exitButton;

    Entity* menuWalpaper;
    Entity* ng;
    Entity* ex;

    Texture* dupa;

    glm::vec2 joystickOffset = glm::vec2(0.0f, 0.0f);
    glm::vec2 joystickOffset2 = glm::vec2(0.0f, 0.0f);
    glm::vec2 joystickOffset3 = glm::vec2(0.0f, 0.0f);
    glm::vec2 joystickOffset4 = glm::vec2(0.0f, 0.0f);

public:
    menuSceneScript(EditorLayer::Editor* editor, AudioManager* audioManager, SceneManager* sm, PlayerInput* playerInput,
                    PlayerInput* playerInput1, DebugInput* debugInput, Shader* shader, Shader* shaderText, Shader* shaderPbr,
                    Shader* screenShader, /*Shader* imageShader,*/ Shader* imageShaderGreen, Renderer* renderer, double lastTime)
            :
            editor(editor),
            audioManager(audioManager),
            sm(sm),
            playerInput(playerInput),
            playerInput1(playerInput1),
            debugInput(debugInput),
            shader(shader),
            shaderText(shaderText),
            shaderPbr(shaderPbr),
            screenShader(screenShader),
            imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl"),
            imageShaderGreen(imageShaderGreen),
            renderer(renderer),
            lastTime(lastTime),
            mainMenu(new Image(&imageShader)),
            startButton(new Button(&imageShader)),
            exitButton(new Button(&imageShader)),
            menuWalpaper(new Entity("mainMenu")),
            ng(new Entity("startButton")),
            ex(new Entity("exitButton")),
            dupa(new Texture("res/content/textures/dupa.png", "dupa"))
    {
    }


    void awake() override{
    };

    void start() override{

        //scene manager
        sm->loadScene("res/content/maps/Kuba.yaml");
        sm->setCurrentScene("KubaScene");


        //main menu
        sm->getSceneByName("KubaScene")->addEntity(menuWalpaper);
        menuWalpaper->addComponent(mainMenu);
        mainMenu->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
        mainMenu->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        mainMenu->setTexture(dupa);




    };

    void update() override{
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput->interpretInput(s.window, s.camera, s.deltaTime);
        //time = time + s.deltaTime;
        deltaTime = s.deltaTime;

        debugInput->interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput->interpretInput();
        playerInput1->interpretInput();

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //shaderText.use();


        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        joystickOffset = playerInput->getJoystick(2);
        joystickOffset2 = playerInput->getJoystick(1);
        joystickOffset3 = playerInput1->getJoystick(2);
        joystickOffset4 = playerInput1->getJoystick(1);

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

        renderer->updateProjectionAndView(projection, view);

        editor->draw();
        sm->updateLoadedScenes();

        glDisable(GL_DEPTH_TEST);
        //imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //hud
        glActiveTexture(GL_TEXTURE0);
        imageShader.use();
        mainMenu->renderPlane();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);


    };

    void onDestroy() override{
        audioManager->end();
    };

    ~menuSceneScript() override = default;
};


#endif
