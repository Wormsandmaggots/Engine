//
// Created by Radek on 26.03.2024.
//

#ifndef ENGINE_EDITOR_H
#define ENGINE_EDITOR_H


#include "Scene/Loader/YamlReader.h"
#include "ECS/Entity.h"
#include "Camera.h"

namespace EditorLayer {

    class Editor {
    public:
        static Editor *Instance;

        Editor();

        void draw();

        void setCamera(Camera*);

    private:
        std::string keyMap = "res/userConfig/keyMaps.yaml";
        Entity *chosenEntity = nullptr;
        Camera *editorCamera;
        YamlReader keyMapReader;

        void drawHierarchy();
        void drawTreeOnChildren(Entity*);
        void chooseEntityWithMouse();
    };

}

#endif //ENGINE_EDITOR_H
