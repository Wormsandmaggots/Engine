//
// Created by Radek on 06.04.2024.
//

#include "Editor/Panels/Inspector.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "Editor/Gizmos.h"
#include "Physics/ColliderComponent.h"

using namespace EditorLayer;

std::unordered_map<std::string, std::function<void(Entity*)>> addComponents = {
        {"ColliderComponent", [](Entity* e) {
            Component* c = new ColliderComponent();
            c->setParent(e);
            e->addComponent(c);}},
        {"Model", [](Entity* e) {
            Component* c = new Model();
            c->setParent(e);
            e->addComponent(c);
        }}
};

void Inspector::draw(Entity* chosenEntity, Camera* editorCamera) {

    char nameBuffer[256];

    glm::mat4 view = editorCamera->GetViewMatrix();
    glm::mat4 projection = editorCamera->GetProjection();

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
    Gizmos::EditTransform(glm::value_ptr(view),
                          glm::value_ptr(projection),
                          chosenEntity);

    ImGui::Separator();

    chosenEntity->drawEditor();

    if(ImGui::Button("Add component"))
        ImGui::OpenPopup("AddComponent");

    if(ImGui::BeginPopup("AddComponent"))
    {
        for (auto element : addComponents) {

            if(ImGui::Selectable(element.first.c_str()))
            {
                element.second(chosenEntity);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}