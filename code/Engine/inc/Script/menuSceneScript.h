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

public:
    // Konstruktor domyślny
    menuSceneScript(EditorLayer::Editor& editor, CollisionManager& cm, SceneManager& sm, SSAO& ssao, Renderer& renderer, AudioManager& audioManager, PlayerInput& playerInput,
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
            buffer(FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT))
    {
    }

    void awake() override{
    };

    void start() override{
        //scene manager
        sm.setCurrentScene("KubaScene");
    };

    void update() override{

        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);

        //time = time + s.deltaTime;

        deltaTime = s.deltaTime;

        debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();

        glClearColor(0.8, 0.8, 0.8, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    };

    void onDestroy() override{
    };

    ~menuSceneScript() override = default;
};


#endif
