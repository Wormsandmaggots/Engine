//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_GIZMOS_H
#define ENGINE_GIZMOS_H

#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.inl"

class Gizmos {
public:
    //Gizmos() {}

    // Metoda do rysowania gizmosów
    static void DrawGizmos(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::mat4& modelMatrix) {
        ImGuizmo::SetRect(0, 0, ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);

        // Wybierz tryb transformacji: translacja, rotacja, skalowanie
        ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
        if (ImGui::RadioButton("Translate", m_currentGizmoOperation == ImGuizmo::TRANSLATE))
            operation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", m_currentGizmoOperation == ImGuizmo::ROTATE))
            operation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", m_currentGizmoOperation == ImGuizmo::SCALE))
            operation = ImGuizmo::SCALE;

        // Wybierz tryb transformacji: lokalny lub globalny
        ImGuizmo::MODE mode = ImGuizmo::LOCAL;
        if (ImGui::RadioButton("Local", m_currentGizmoMode == ImGuizmo::LOCAL))
            mode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", m_currentGizmoMode == ImGuizmo::WORLD))
            mode = ImGuizmo::WORLD;

        float matrix[16];
        memcpy(matrix, &modelMatrix, sizeof(float) * 16);

        ImGuizmo::Manipulate(reinterpret_cast<const float *>(&viewMatrix),
                             reinterpret_cast<const float *>(&projectionMatrix), operation, mode, matrix);

        memcpy(&modelMatrix, matrix, sizeof(float) * 16);
    }

private:
    static ImGuizmo::OPERATION m_currentGizmoOperation;
    static ImGuizmo::MODE m_currentGizmoMode;
};


#endif //ENGINE_GIZMOS_H
