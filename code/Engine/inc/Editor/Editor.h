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

        void drawHierarchy();
        void setCamera(Camera*);
        void drawTreeOnChildren(Entity*);

    private:
        std::string keyMap = "res/userConfig/keyMaps.yaml";
        Entity *chosenEntity = nullptr;
        Camera *editorCamera;
        YamlReader keyMapReader;
    };

}

#endif //ENGINE_EDITOR_H
