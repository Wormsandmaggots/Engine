#ifndef ENGINE_BASESCENESCRIPT_H
#define ENGINE_BASESCENESCRIPT_H

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

class BaseSceneScript : public SceneScript {
protected:
    EditorLayer::Editor editor;
    SceneManager sm;

    AudioManager& audioManager;
    std::shared_ptr<Sound> sound;

    int connectedControllers;
    PlayerInput playerInput;
    PlayerInput playerInput1;
    glm::vec2 joystickOffset;
    glm::vec2 joystickOffset2;
    glm::vec2 joystickOffset3;
    glm::vec2 joystickOffset4;

    DebugInput debugInput;

    Shader shader;
    Shader shaderText;
    Shader shaderPbr;
    Shader screenShader;
    Shader imageShader;
    Shader imageShaderGreen;

    Renderer renderer;

    double lastTime;

public:
    BaseSceneScript() :
            audioManager(AudioManager::getInstance()),
            sound(audioManager.loadSound("res/content/sounds/songs/queen.wav")),
            connectedControllers(JslConnectDevices()),
            playerInput(GLFW_JOYSTICK_1),
            playerInput1(GLFW_JOYSTICK_2),
            joystickOffset(glm::vec2(0)),
            joystickOffset2(glm::vec2(0)),
            joystickOffset3(glm::vec2(0)),
            joystickOffset4(glm::vec2(0)),
            shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl"),
            shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl"),
            shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl"),
            screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag"),
            imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl"),
            imageShaderGreen("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_green.glsl"),
            renderer(&shader),
            resBar(new ResizableImage(&imageShaderGreen)),
            resBarEntity(new Entity("resBar")),
            lastTime(0.0)
    {
    }

    virtual void initCommonElements() {
        editor.init(&s.camera);
        audioManager.init();
        renderer.init();
        screenShader.use();
        screenShader.setInt("screenTexture", 0);
        sm.getSceneByName("KubaScene")->addEntity(resBarEntity);
        resBarEntity->addComponent(resBar);
        resBar->getTransform()->setScale(glm::vec3(0.02f, 0.3f, 0.0f));
        resBar->getTransform()->setPosition(glm::vec3(0.847f, 0.0f, 0.0f));
        AudioManager::getInstance().playSound("res/content/sounds/songs/queen.wav", 1.0f);
    }

    virtual void updateCommonElements() {
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        deltaTime = s.deltaTime;
        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();
        joystickOffset = playerInput.getJoystick(2);
        joystickOffset2 = playerInput.getJoystick(1);
        joystickOffset3 = playerInput1.getJoystick(2);
        joystickOffset4 = playerInput1.getJoystick(1);
        joystickOffset.x = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset.x, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset.y = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset.y, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset2.x = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset2.x, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset2.y = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset2.y, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset3.x = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset3.x, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset3.y = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset3.y, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset4.x = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset4.x, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset4.y = Math::Remap(utils::easeInOutQuint(Math::Remap(joystickOffset4.y, -1, 1, 0, 1)), 0, 1, -1, 1);
        joystickOffset *= 200 * s.deltaTime;
        joystickOffset2 *= 200 * s.deltaTime;
        joystickOffset3 *= 200 * s.deltaTime;
        joystickOffset4 *= 200 * s.deltaTime;
        renderer.updateProjectionAndView(projection, view);
        editor.draw();
        sm.updateLoadedScenes();
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glActiveTexture(GL_TEXTURE0);
        imageShader.use();
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }

    virtual void destroyCommonElements() {
        audioManager.end();
    }

    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void onDestroy() = 0;
    ~BaseSceneScript() override = default;
};


#endif //ENGINE_BASESCENESCRIPT_H
