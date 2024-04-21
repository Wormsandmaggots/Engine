//
// Created by Radek on 26.03.2024.
//

#ifndef ENGINE_EDITOR_H
#define ENGINE_EDITOR_H


#include "Scene/Loader/YamlReader.h"
#include "ECS/Entity.h"
#include "Camera.h"
#include "Scene/Scene.h"
#include "Editor/Panels/Hierarchy.h"

namespace EditorLayer {

    class Editor {
    public:
        static Editor *Instance;

        Editor();

        void init(Camera*);
        void end();

        void draw();

        void setCamera(Camera*);

    private:
        std::string keyMap = "res/userConfig/keyMaps.yaml";
        Camera *editorCamera = nullptr;
        YamlReader keyMapReader;
        Hierarchy *hierarchy = new Hierarchy();

        void chooseEntityWithMouse();
        void drawMainMenu();
    };

}

#endif //ENGINE_EDITOR_H
