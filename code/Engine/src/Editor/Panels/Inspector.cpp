//
// Created by Radek on 06.04.2024.
//

#include "Editor/Panels/Inspector.h"
#include "imgui.h"
#include "ImGuizmo.h"
#include "Editor/Gizmos.h"

using namespace EditorLayer;

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
                          chosenEntity->getTransform());

    ImGui::End();
}
