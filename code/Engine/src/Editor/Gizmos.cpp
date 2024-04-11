//
// Created by Radek on 25.03.2024.
//

#include "Editor/Gizmos.h"
#include "Shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

using namespace EditorLayer;

ImGuizmo::OPERATION Gizmos::CurrentGizmoOperation = ImGuizmo::TRANSLATE;
ImGuizmo::MODE Gizmos::CurrentGizmoMode = ImGuizmo::LOCAL;

void Gizmos::EditTransform(float* viewMatrix, float* projectionMatrix, Transform2* transform)
{
    float* modelMatrix = CurrentGizmoMode == ImGuizmo::LOCAL ? glm::value_ptr(transform->getLocalMatrix()) : glm::value_ptr(transform->getWorldMatrix());

    //TODO: after input module, change this
    if (ImGui::IsKeyPressed('T'))
        CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed('R'))
        CurrentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed('Y'))
        CurrentGizmoOperation = ImGuizmo::SCALE;

    if (ImGui::RadioButton("Translate", CurrentGizmoOperation == ImGuizmo::TRANSLATE))
        CurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", CurrentGizmoOperation == ImGuizmo::ROTATE))
        CurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", CurrentGizmoOperation == ImGuizmo::SCALE))
        CurrentGizmoOperation = ImGuizmo::SCALE;

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(modelMatrix, matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Position", matrixTranslation, "%.3f");
    ImGui::InputFloat3("Rotation", matrixRotation, "%.3f");
    ImGui::InputFloat3("Scale", matrixScale, "%.3f");
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, modelMatrix);

    if (CurrentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", CurrentGizmoMode == ImGuizmo::LOCAL))
            CurrentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", CurrentGizmoMode == ImGuizmo::WORLD))
            CurrentGizmoMode = ImGuizmo::WORLD;
    }

//    static bool useSnap(false);
//    if (ImGui::IsKeyPressed(83))
//        useSnap = !useSnap;
//    ImGui::Checkbox("", &useSnap);
//    ImGui::SameLine();
//    vec_t snap;
//    switch (mCurrentGizmoOperation)
//    {
//        case ImGuizmo::TRANSLATE:
//            snap = config.mSnapTranslation;
//            ImGui::InputFloat3("Snap", &snap.x);
//            break;
//        case ImGuizmo::ROTATE:
//            snap = config.mSnapRotation;
//            ImGui::InputFloat("Angle Snap", &snap.x);
//            break;
//        case ImGuizmo::SCALE:
//            snap = config.mSnapScale;
//            ImGui::InputFloat("Scale Snap", &snap.x);
//            break;
//    }

    ImGuiIO& io = ImGui::GetIO();
    ImGuizmo::SetRect(0, 0,io.DisplaySize.x, io.DisplaySize.y);

    float matrix[16];
    memcpy(matrix, modelMatrix, sizeof(float) * 16);

    ImGuizmo::Manipulate(viewMatrix,
                         projectionMatrix,
                         CurrentGizmoOperation,
                         CurrentGizmoMode,
                         matrix);

    if (ImGuizmo::IsUsing()) {
        // Update object model matrix if gizmo is being used
        memcpy(modelMatrix, matrix, sizeof(float) * 16);

        if(CurrentGizmoMode == ImGuizmo::LOCAL)
        {
            ImGuizmo::DecomposeMatrixToComponents(modelMatrix, matrixTranslation, matrixRotation, matrixScale);
            //transform->setLocalTransform(*modelMatrix);
            transform->setPosition(reinterpret_cast<const glm::vec<3, float> &>(matrixTranslation));
            transform->setRotation(reinterpret_cast<const glm::vec<3, float> &>(matrixRotation));
            transform->setScale(reinterpret_cast<const glm::vec<3, float> &>(matrixScale));
        }
        else
        {
            transform->setTransform(*modelMatrix);
        }
    }

}