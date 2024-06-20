#ifndef ENGINE_SCENESCRIPT_H
#define ENGINE_SCENESCRIPT_H

#include "glm/fwd.hpp"
#include "DependencyContainer.h"
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

using namespace SceneManagement;

class SceneScript{
protected:

private:

public:
    SceneScript() = default;

    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void onDestroy() = 0;
    virtual ~SceneScript() = default;

};

#endif
