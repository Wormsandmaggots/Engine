//
// Created by Radek on 06.04.2024.
//

#ifndef ENGINE_HIERARCHY_H
#define ENGINE_HIERARCHY_H

#include <unordered_map>
#include <functional>
#include "imgui.h"
#include "Scene/Scene.h"
#include "ECS/Entity.h"
#include "Editor/Camera.h"

class Hierarchy {
public:
    Hierarchy();
    virtual ~Hierarchy() = default;

    void draw(Camera*);

private:

    //imgui flags for displaying tree nodes
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    //chosen is set after clicking on one node
    Entity *chosenEntity = nullptr;
    Scene *chosenScene = nullptr;

    //obviously after dragging
    Entity *draggedEntity = nullptr;

    //for a popup menu
    Entity* contextMenuEntity = nullptr;
    Scene* contextMenuScene = nullptr;
    bool showContextPopup = false;
    std::unordered_map<std::string, std::function<void(void)>> contextMenuActions;

    void drawTreeNode(Entity*, Scene*);
    void drawContextMenuPopup();
};


#endif //ENGINE_HIERARCHY_H
