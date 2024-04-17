//
// Created by Radek on 25.03.2024.
//

#include "Editor/Gizmos.h"
#include "Shader.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "ECS/Entity.h"
#include "Core/Utils/StringUtils.h"

using namespace EditorLayer;


ImGuizmo::OPERATION Gizmos::CurrentGizmoOperation = ImGuizmo::TRANSLATE;
ImGuizmo::MODE Gizmos::CurrentGizmoMode = ImGuizmo::WORLD;

void Gizmos::EditTransform(float* viewMatrix, float* projectionMatrix, Entity* entity)
{
    static Entity* cachedEntity = entity;
    static Transform2* transform = cachedEntity->getTransform();
    static glm::mat4* world = new glm::mat4(transform->getWorldMatrix());
    static glm::mat4* local = new glm::mat4(transform->getLocalMatrix());
    static float e = 0.01f;

    bool changed = false;

    if(entity != cachedEntity)
    {
        delete world;
        delete local;

        cachedEntity = entity;
        transform = cachedEntity->getTransform();
        world = new glm::mat4(transform->getWorldMatrix());
        local = new glm::mat4(transform->getLocalMatrix());
    }

    float* modelMatrix = CurrentGizmoMode == ImGuizmo::LOCAL ? glm::value_ptr(*local) : glm::value_ptr(*world);

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

    //ADD IF
    if(ImGui::DragFloat3("Position", matrixTranslation, 0.1f))
    {
        transform->setPosition(glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]));
        changed = true;
    }

    if(ImGui::DragFloat3("Rotation", matrixRotation, 0.1f))
    {
        transform->setRotation(glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]));
        changed = true;
    }

    if(ImGui::DragFloat3("Scale", matrixScale, 0.1f))
    {
        transform->setScale(glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]));
        changed = true;
    }


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

    memcpy(matrix, world, sizeof(float) * 16);

    glm::mat4 delta;

    ImGuizmo::Manipulate(viewMatrix,
                         projectionMatrix,
                         CurrentGizmoOperation,
                         CurrentGizmoMode,
                         matrix, glm::value_ptr(delta));

    glm::vec2 crMouseDelta = glm::vec2(io.MouseDelta.x, io.MouseDelta.y);

    if (ImGuizmo::IsUsing()) {

        ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(delta),matrixTranslation, matrixRotation, matrixScale);

        glm::vec3 tr = glm::vec3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
        glm::vec3 rt = glm::vec3(matrixRotation[0], matrixRotation[1], matrixRotation[2]);
        glm::vec3 sc = glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]);

        if(CurrentGizmoOperation == ImGuizmo::TRANSLATE && glm::length(tr) > e)
        {
            transform->translate(tr);
            changed = true;
        }

        if(CurrentGizmoOperation == ImGuizmo::ROTATE && glm::length(rt) > e)
        {
            transform->rotate(rt);
            changed = true;
        }

        if(CurrentGizmoOperation == ImGuizmo::SCALE && glm::length(sc) > e)
        {
            ImGuizmo::DecomposeMatrixToComponents(matrix,matrixTranslation, matrixRotation, matrixScale);
            //LOG_INFO(utils::mat4ToString(delta));
            transform->setScale(glm::vec3(matrixScale[0], matrixScale[1], matrixScale[2]));
            changed = true;
        }
    }

    if(changed)
    {
        cachedEntity->setDirtyTree();

        cachedEntity->update();

        delete local;
        delete world;

        local = new glm::mat4(transform->getLocalMatrix());
        world = new glm::mat4(transform->getWorldMatrix());
    }
}