//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_GIZMOS_H
#define ENGINE_GIZMOS_H

#include <vector>
#include "imgui.h"
#include "ImGuizmo.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.inl"
#include "glm/detail/type_mat4x4.hpp"
#include "Scene/Transform2.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "Shader.h"

namespace EditorLayer {

    class Gizmos {
    public:
        static ImGuizmo::OPERATION CurrentGizmoOperation;
        static ImGuizmo::MODE CurrentGizmoMode;

        static void EditTransform(float *viewMatrix, float *projectionMatrix, Transform2*);
    };

}


#endif //ENGINE_GIZMOS_H
