//
// Created by Radek on 06.04.2024.
//

#include "Editor/Panels/Hierarchy.h"
#include "Scene/SceneManager.h"
#include "Editor/Gizmos.h"
#include "Editor/Panels/Inspector.h"

using namespace EditorLayer;

void Hierarchy::draw(Camera* editorCamera) {
    ImGui::ShowDemoWindow();
    ImGui::Begin("Hierarchy");

    for(Scene2* scene : SceneManagement::SceneManager::Instance->getLoadedScenes()) {
        ImGuiTreeNodeFlags currentFlags = flags;

        if (ImGui::TreeNodeEx(scene->getName().c_str(), currentFlags))
        {
            if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                chosenScene = scene;
                chosenEntity = nullptr;
            }
            else if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {

                showContextPopup = true;
                contextMenuScene = scene;
                contextMenuEntity = nullptr;
            }

            if (ImGui::BeginDragDropTarget())
            {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggedEntity->getName().c_str()))
                {
                    //if entity doesnt have parent
                    //it means it is a root of a scene
                    if(draggedEntity->getParent())
                    {
                        draggedEntity->getParent()->removeChild(draggedEntity);
                        scene->addEntity(draggedEntity);
                    }
                }

                ImGui::EndDragDropTarget();
            }

            for (Entity* e : scene->getSceneEntities()) {
                drawTreeNode(e, scene);
            }

            ImGui::TreePop();
        }
        else
        {
            if(ImGui::IsItemClicked(ImGuiMouseButton_Left))
            {
                chosenScene = scene;
                chosenEntity = nullptr;
            }
            else if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                showContextPopup = true;
                contextMenuScene = scene;
                contextMenuEntity = nullptr;
            }

            if (ImGui::BeginDragDropTarget())
            {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggedEntity->getName().c_str()))
                {
                    draggedEntity->getParent()->removeChild(draggedEntity);
                    scene->addEntity(draggedEntity);
                }

                ImGui::EndDragDropTarget();
            }
        }

    }

    ImGui::End();

    if(chosenEntity != nullptr)
    {
        Inspector::draw(chosenEntity, editorCamera);
    }

    if(showContextPopup)
    {
        drawContextMenuPopup();
    }
}

void Hierarchy::drawTreeNode(Entity* parent, Scene2* currentScene)
{
    ImGuiTreeNodeFlags currentFlags = flags;

    if(parent->getChildren().empty())
    {
        currentFlags |= ImGuiTreeNodeFlags_Leaf;
    }

    if(parent == chosenEntity)
    {
        currentFlags |= ImGuiTreeNodeFlags_Selected;
    }

    //!label doesnt have to be unique
    if (ImGui::TreeNodeEx(parent->getName().c_str(), currentFlags))
    {
        if (ImGui::BeginDragDropSource())
        {
            draggedEntity = parent;
            ImGui::SetDragDropPayload(draggedEntity->getName().c_str(), draggedEntity, sizeof(draggedEntity));
            ImGui::Text(draggedEntity->getName().c_str());
            ImGui::EndDragDropSource();
        }
            //!Change it to time dependent
        else if(ImGui::IsItemClicked())
        {
            chosenEntity = parent;
            chosenScene = currentScene;
        }
        else if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            showContextPopup = true;
            contextMenuEntity = parent;
        }

        if (ImGui::BeginDragDropTarget())
        {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggedEntity->getName().c_str()))
            {
                //draggedEntity->getParent()->removeChild(draggedEntity);
                if(draggedEntity->getParent() == nullptr)
                {
                    currentScene->removeEntity(draggedEntity);
                }

                parent->addChild(draggedEntity);
            }

            ImGui::EndDragDropTarget();
        }

        for (Entity* e : parent->getChildren()) {
            drawTreeNode(e, currentScene);
        }

        ImGui::TreePop();
    }
    else
    {
        if (ImGui::BeginDragDropSource())
        {
            draggedEntity = parent;
            ImGui::SetDragDropPayload(draggedEntity->getName().c_str(), draggedEntity, sizeof(draggedEntity));
            ImGui::Text(draggedEntity->getName().c_str());
            ImGui::EndDragDropSource();
        }
            //!Change it to time dependent
        else if(ImGui::IsItemClicked())
        {
            chosenEntity = parent;
            chosenScene = currentScene;
        }
        else if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            showContextPopup = true;
            contextMenuEntity = parent;
            contextMenuScene = nullptr;
        }

        if (ImGui::BeginDragDropTarget())
        {
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggedEntity->getName().c_str()))
            {
                if(draggedEntity->getParent() == nullptr)
                {
                    currentScene->removeEntity(draggedEntity);
                }

                parent->addChild(draggedEntity);
            }

            ImGui::EndDragDropTarget();
        }
    }
}

Hierarchy::Hierarchy() {
    contextMenuActions = {
            {"Add child", [this] () {
                if(contextMenuEntity == nullptr)
                {
                    contextMenuScene->addEntity(new Entity());
                }
                else
                {
                    contextMenuEntity->addChild(new Entity());
                }
            }},
            {"Delete",    [this] () {
                if(contextMenuEntity == nullptr) return;

                if(chosenEntity != nullptr && contextMenuEntity == chosenEntity)
                {
                    chosenEntity = nullptr;
                }

                if(contextMenuEntity->getParent() != nullptr)
                {
                    contextMenuEntity->getParent()->removeChild(contextMenuEntity);
                    delete contextMenuEntity;
                }
                else
                {
                    chosenScene->removeEntity(contextMenuEntity);
                    chosenScene = nullptr;
                    delete contextMenuEntity;
                }
            }},
            {"Temp close", [this] () { showContextPopup = false; }}
    };
}

void Hierarchy::drawContextMenuPopup() {
    ImGui::OpenPopup("ContextPopup");

    ImGui::SameLine();
    if (ImGui::BeginPopup("ContextPopup")) {
        ImGui::Text(contextMenuEntity != nullptr ? contextMenuEntity->getName().c_str() : contextMenuScene->getName().c_str());
        ImGui::Separator();
        for (auto action : contextMenuActions)
            if (ImGui::Selectable(action.first.c_str()))
            {
                showContextPopup = false;
                action.second();
            }
        ImGui::EndPopup();
    }
}
