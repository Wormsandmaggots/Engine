//
// Created by Radek on 25.03.2024.
//

#include "Editor/Gizmos.h"

ImGuizmo::OPERATION Gizmos::currentGizmoOperation = ImGuizmo::TRANSLATE;
ImGuizmo::MODE Gizmos::currentGizmoMode = ImGuizmo::LOCAL;

void Gizmos::editTransform(float* viewMatrix, float* projectionMatrix, float* modelMatrix)
{

    if (ImGui::IsKeyPressed('T'))
        currentGizmoOperation = ImGuizmo::TRANSLATE;
    if (ImGui::IsKeyPressed('R'))
        currentGizmoOperation = ImGuizmo::ROTATE;
    if (ImGui::IsKeyPressed('Y'))
        currentGizmoOperation = ImGuizmo::SCALE;

    if (ImGui::RadioButton("Translate", currentGizmoOperation == ImGuizmo::TRANSLATE))
        currentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", currentGizmoOperation == ImGuizmo::ROTATE))
        currentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", currentGizmoOperation == ImGuizmo::SCALE))
        currentGizmoOperation = ImGuizmo::SCALE;

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(modelMatrix, matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Tr", matrixTranslation, "%.3f");
    ImGui::InputFloat3("Rt", matrixRotation, "%.3f");
    ImGui::InputFloat3("Sc", matrixScale, "%.3f");
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, modelMatrix);

    if (currentGizmoOperation != ImGuizmo::SCALE)
    {
        if (ImGui::RadioButton("Local", currentGizmoMode == ImGuizmo::LOCAL))
            currentGizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", currentGizmoMode == ImGuizmo::WORLD))
            currentGizmoMode = ImGuizmo::WORLD;
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
                         currentGizmoOperation,
                         currentGizmoMode,
                         matrix);

    if (ImGuizmo::IsUsing()) {
        // Update object model matrix if gizmo is being used
        memcpy(modelMatrix, matrix, sizeof(float) * 16);
    }

}