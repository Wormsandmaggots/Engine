//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_GIZMOS_H
#define ENGINE_GIZMOS_H

#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.inl"
#include "glm/detail/type_mat4x4.hpp"

class Gizmos {
public:
    static void editTransform(float* viewMatrix, float* projectionMatrix, float* modelMatrix);

private:
    static ImGuizmo::OPERATION currentGizmoOperation;
    static ImGuizmo::MODE currentGizmoMode;
};


#endif //ENGINE_GIZMOS_H