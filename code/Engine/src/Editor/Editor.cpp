//
// Created by Radek on 26.03.2024.
//

#include "imgui.h"
#include "Editor/Gizmos.h"
#include "Editor/Editor.h"
#include "Scene/SceneManager.h"


using namespace EditorLayer;


Editor* Editor::Instance = nullptr;


ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;


bool showContextPopup = false;
Entity* contextMenuEntity = nullptr;
Scene2* contextMenuScene = nullptr;

Entity* draggedEntity = nullptr;

//!Shared pointers for a rescue
//!FALSE ALARM (FOR NOW)


Editor::Editor() {
    if(Editor::Instance == nullptr)
    {
        Editor::Instance = this;
        setContextActions();
    }
    else
    {
        delete this;
    }

    keyMapReader.parseFile(keyMap);
}

void Editor::draw() {
    drawHierarchy();
    chooseEntityWithMouse();
}

void Editor::drawTreeOnChildren(Entity* parent)
{
    for (Entity* e : parent->getChildren()) {

        ImGuiTreeNodeFlags currentFlags = flags;

        if(e->getChildren().empty())
        {
            currentFlags |= ImGuiTreeNodeFlags_Leaf;
        }

        if(ImGui::TreeNodeEx(e->getName().c_str(), currentFlags))
        {
            if (ImGui::BeginDragDropSource())
            {
                draggedEntity = e;
                ImGui::SetDragDropPayload(draggedEntity->getName().c_str(), draggedEntity, sizeof(draggedEntity));
                ImGui::Text(draggedEntity->getName().c_str());
                ImGui::EndDragDropSource();
            }
            //!Change it to time dependent
            else if(ImGui::IsItemClicked())
            {
                chosenEntity = e;
            }
            else if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                showContextPopup = true;
                contextMenuEntity = e;
            }

            if(!e->getChildren().empty())
            {
                drawTreeOnChildren(e);
            }

            ImGui::TreePop();
        }
        else
        {
            if (ImGui::BeginDragDropSource())
            {
                draggedEntity = e;
                ImGui::SetDragDropPayload(draggedEntity->getName().c_str(), draggedEntity, 0);
                ImGui::Text(draggedEntity->getName().c_str());
                ImGui::EndDragDropSource();
            }
            else if(ImGui::IsItemClicked())
            {
                chosenEntity = e;
            }
            else if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
            {
                showContextPopup = true;
                contextMenuEntity = e;
            }
        }
    }
}

void Editor::draw(Entity* parent, Scene2* currentScene)
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
            draw(e, currentScene);
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

void Editor::drawHierarchy() {

    ImGui::ShowDemoWindow();
    ImGui::Begin("Hierarchy");

    for(Scene2* scene : SceneManagement::SceneManager::Instance->getLoadedScenes()) {
        ImGuiTreeNodeFlags currentFlags = flags;

        if (ImGui::TreeNodeEx(scene->getName().c_str(), currentFlags))
        {
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
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
                draw(e, scene);
            }

            ImGui::TreePop();
        }
        else
        {
            if(ImGui::IsItemClicked(ImGuiMouseButton_Right))
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

    glm::mat4 view = editorCamera->GetViewMatrix();
    glm::mat4 projection = editorCamera->GetProjection();

    if(chosenEntity != nullptr)
    {
        char nameBuffer[256];
        strcpy(nameBuffer, chosenEntity->getName().c_str());

        ImGui::Begin("Inspector");
        // ## - gives empty label
        if (ImGui::InputText("##", nameBuffer, sizeof(nameBuffer))) {
            if(strcmp(nameBuffer,"\0") != 0)
            {
                chosenEntity->setName(nameBuffer);
            }
        }
        ImGui::NewLine();
        ImGuizmo::BeginFrame();
        Gizmos::editTransform(glm::value_ptr(view),
                              glm::value_ptr(projection),
                              glm::value_ptr(Gizmos::CurrentGizmoMode == ImGuizmo::MODE::WORLD ?
                                             chosenEntity->getTransform()->getWorldMatrix() :
                                             chosenEntity->getTransform()->getLocalMatrix()));

        ImGui::End();
    }

    if(showContextPopup)
    {
        ImGui::OpenPopup("ContextPopup");

        ImGui::SameLine();
        if (ImGui::BeginPopup("ContextPopup")) {
            ImGui::Text(contextMenuEntity != nullptr ? contextMenuEntity->getName().c_str() : contextMenuScene->getName().c_str());
            ImGui::Separator();
            for (auto action : contextMenuActions)
                if (ImGui::Selectable(action.first.c_str()))
                {
                    showContextPopup = false;
                    action.second(contextMenuEntity);
                }
            ImGui::EndPopup();
        }
    }
}


void Editor::setCamera(Camera * newCamera) {
    editorCamera = newCamera;
}

void Editor::chooseEntityWithMouse() {
    auto[mx, my] = ImGui::GetMousePos();
    int mouseX = (int)mx;
    int mouseY = (int)my;

    if (mouseX >= 0 && mouseY >= 0)
    {
        int pixelData;

        glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        //LOG_INFO(to_string(pixelData));
//        if(pixelData != -1)
//        {
//
//        }
    }
}

void Editor::setContextActions() {
    contextMenuActions =
            {
                    {"Add child", [] (Entity* e) {
                        if(e == nullptr)
                        {
                            contextMenuScene->addEntity(new Entity());
                        }
                        else
                        {
                            e->addChild(new Entity());
                        }
                    }},
                    {"Delete",    [this] (Entity* e) {
                        if(e == nullptr) return;

                        if(chosenEntity != nullptr && e == chosenEntity)
                        {
                            chosenEntity = nullptr;
                        }

                        if(e->getParent() != nullptr)
                        {
                            e->getParent()->removeChild(e);
                            delete e;
                        }
                        else
                        {
                            chosenScene->removeEntity(e);
                            chosenScene = nullptr;
                            delete e;
                        }
                    }},
                    {"Temp close", [] (Entity* e) { showContextPopup = false; }}
            };
}
