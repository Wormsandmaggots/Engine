#ifndef ENGINE_DEPENDENCYCONTAINER_H
#define ENGINE_DEPENDENCYCONTAINER_H

#include "windows.h"
#include "Debug/Profiler.h"
#include "tracy/TracyOpenGL.hpp"
#include "Text/Text.h"
#include "Engine/inc/Engine.h"
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
#include "Globals.h"
#include "Input/DebugInput.h"
#include "Input/PlayerInput.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "RigPrep.h"
#include "Animation/InverseKinematics.h"
#include "ECS/ScriptableEntity.h"


class DependencyContainer {
public:
    //Shader shader;
    //SSAO ssao;
    //Renderer renderer;
    //Editor editor;

    DependencyContainer() {
        // Tutaj inicjalizujesz swoje zależności
        //shader = Shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
        //ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        //renderer = Renderer(&ssao.shaderGeometryPass);
        //renderer.init();
        //editor.init(&s.camera);
    }
};

#endif