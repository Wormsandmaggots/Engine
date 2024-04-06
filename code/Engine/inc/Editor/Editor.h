//
// Created by Radek on 26.03.2024.
//

#ifndef ENGINE_EDITOR_H
#define ENGINE_EDITOR_H


#include "Scene/Loader/YamlReader.h"
#include "ECS/Entity.h"
#include "Camera.h"
#include "Scene/Scene2.h"

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
        Scene2 *chosenScene = nullptr;
        Camera *editorCamera;
        YamlReader keyMapReader;
        std::unordered_map<std::string, std::function<void(Entity*)>> contextMenuActions;

        void drawHierarchy();
        void drawTreeOnChildren(Entity*);
        void draw(Entity*, Scene2*);
        void chooseEntityWithMouse();
        void setContextActions();
    };

}

#endif //ENGINE_EDITOR_H
