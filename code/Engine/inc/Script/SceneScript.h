#ifndef ENGINE_SCENESCRIPT_H
#define ENGINE_SCENESCRIPT_H

#include "glm/fwd.hpp"
#include "DependencyContainer.h"

#include "Engine/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"
#include "Script/menuSceneScript.h"

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

class SceneScript{
protected:
    static EditorLayer::Editor editor;
    static CollisionManager cm;
    static SceneManager sm;
    static AudioManager& audioManager;

    static PlayerInput playerInput;
    static PlayerInput playerInput1;
    static DebugInput debugInput;

    static Shader shader;
    static Shader collisionTestShader;
    static Shader shaderText;
    static Shader colorShader;
    static Shader shaderPbr;
    static Shader screenShader;
    static Shader shaderRig;
    static Shader shaderBarmanRig;
    static Shader DrunkShader;
    static Shader shaderNoneDrink;
    static Shader reverseShader;
    static Shader imageShader;
    static Shader imageShaderGreen;
    static Shader shaderRigInstanced;

    static SSAO ssao;

    static Renderer renderer;
public:
    SceneScript() = default;

    static void mainInit() {
        audioManager.init();
        sm.loadScene("res/content/maps/Kuba.yaml");
        sm.loadScene("res/content/maps/Marcin.yaml");
        ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        renderer.init();
        editor.init(&s.camera);
    }

    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void onDestroy() = 0;
    virtual ~SceneScript() = default;

};

// Definicje statycznych członków klasy
EditorLayer::Editor SceneScript::editor;
CollisionManager SceneScript::cm;
SceneManager SceneScript::sm;
AudioManager& SceneScript::audioManager = AudioManager::getInstance();

PlayerInput SceneScript::playerInput(GLFW_JOYSTICK_1);
PlayerInput SceneScript::playerInput1(GLFW_JOYSTICK_2);
DebugInput SceneScript::debugInput;

Shader SceneScript::shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
Shader SceneScript::collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
Shader SceneScript::shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
Shader SceneScript::colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
Shader SceneScript::shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
Shader SceneScript::screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");
Shader SceneScript::shaderRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag");
Shader SceneScript::shaderBarmanRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/SSAO/ssao_fragment.frag");
Shader SceneScript::DrunkShader("res/content/shaders/SSAO/ssao.vert", "res/content/shaders/chromaticAberration.frag");
Shader SceneScript::shaderNoneDrink("res/content/shaders/SSAO/ssao.vert", "res/content/shaders/framebuffer.frag");
Shader SceneScript::reverseShader("res/content/shaders/SSAO/ssao.vert","res/content/shaders/reverse.frag");
Shader SceneScript::imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl");
Shader SceneScript::imageShaderGreen("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_green.glsl");
Shader SceneScript::shaderRigInstanced("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/SSAO/ssao_fragment.frag");

SSAO SceneScript::ssao;

Renderer SceneScript::renderer(&SceneScript::ssao.shaderGeometryPass);

#endif
