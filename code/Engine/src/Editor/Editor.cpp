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


Editor::Editor() {
    if(Editor::Instance == nullptr)
    {
        Editor::Instance = this;
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
            if(ImGui::IsItemClicked())
            {
                chosenEntity = e;
            }

            if(!e->getChildren().empty())
            {
                drawTreeOnChildren(e);
            }

            ImGui::TreePop();
        }
        else
        {
            if(ImGui::IsItemClicked())
            {
                chosenEntity = e;
            }
        }
    }
}

void Editor::drawHierarchy() {
    ImGui::Begin("Hierarchy");

    for(Scene2* scene : SceneManagement::SceneManager::Instance->getLoadedScenes())
    {
        ImGuiTreeNodeFlags currentFlags = flags;

        if(scene->getRoot()->getChildren().empty())
        {
            currentFlags |= ImGuiTreeNodeFlags_Leaf;
        }

        if(ImGui::TreeNodeEx(scene->getRoot()->getName().c_str(), currentFlags))
        {
            if(ImGui::IsItemClicked())
            {
                chosenEntity = scene->getRoot();
            }

            for (Entity* e : scene->getSceneEntities()) {
                drawTreeOnChildren(e);
            }

            ImGui::TreePop();
        }
        else
        {
            if(ImGui::IsItemClicked())
            {
                chosenEntity = scene->getRoot();
            }
        }
    }

    ImGui::End();

    glm::mat4 view = editorCamera->GetViewMatrix();
    glm::mat4 projection = editorCamera->GetProjection();

    if(chosenEntity == nullptr) return;

    ImGui::Begin("Inspector");
    ImGui::Text(chosenEntity->getName().c_str());
    ImGui::NewLine();
    ImGuizmo::BeginFrame();
    Gizmos::editTransform(glm::value_ptr(view),
                          glm::value_ptr(projection),
                          glm::value_ptr(Gizmos::CurrentGizmoMode == ImGuizmo::MODE::WORLD ?
                          chosenEntity->getTransform()->getWorldMatrix() :
                          chosenEntity->getTransform()->getLocalMatrix()));
    ImGui::End();
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

        glReadBuffer(GL_COLOR_ATTACHMENT0 + 1);
        glReadPixels(mouseX, mouseY, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        LOG_INFO(to_string(pixelData));
//        if(pixelData != -1)
//        {
//
//        }
    }
}
