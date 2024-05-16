#ifndef ENGINE_FRUSTUM_H
#define ENGINE_FRUSTUM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Frustum {
public:
    glm::vec4 planes[6];

    void update(const glm::mat4& viewProjectionMatrix) {
        // Prawa płaszczyzna
        planes[0] = glm::vec4(
                viewProjectionMatrix[0][3] - viewProjectionMatrix[0][0],
                viewProjectionMatrix[1][3] - viewProjectionMatrix[1][0],
                viewProjectionMatrix[2][3] - viewProjectionMatrix[2][0],
                viewProjectionMatrix[3][3] - viewProjectionMatrix[3][0]
        );

        // Lewa płaszczyzna
        planes[1] = glm::vec4(
                viewProjectionMatrix[0][3] + viewProjectionMatrix[0][0],
                viewProjectionMatrix[1][3] + viewProjectionMatrix[1][0],
                viewProjectionMatrix[2][3] + viewProjectionMatrix[2][0],
                viewProjectionMatrix[3][3] + viewProjectionMatrix[3][0]
        );

        // Dolna płaszczyzna
        planes[2] = glm::vec4(
                viewProjectionMatrix[0][3] + viewProjectionMatrix[0][1],
                viewProjectionMatrix[1][3] + viewProjectionMatrix[1][1],
                viewProjectionMatrix[2][3] + viewProjectionMatrix[2][1],
                viewProjectionMatrix[3][3] + viewProjectionMatrix[3][1]
        );

        // Górna płaszczyzna
        planes[3] = glm::vec4(
                viewProjectionMatrix[0][3] - viewProjectionMatrix[0][1],
                viewProjectionMatrix[1][3] - viewProjectionMatrix[1][1],
                viewProjectionMatrix[2][3] - viewProjectionMatrix[2][1],
                viewProjectionMatrix[3][3] - viewProjectionMatrix[3][1]
        );

        // Daleka płaszczyzna
        planes[4] = glm::vec4(
                viewProjectionMatrix[0][3] - viewProjectionMatrix[0][2],
                viewProjectionMatrix[1][3] - viewProjectionMatrix[1][2],
                viewProjectionMatrix[2][3] - viewProjectionMatrix[2][2],
                viewProjectionMatrix[3][3] - viewProjectionMatrix[3][2]
        );

        // Bliska płaszczyzna
        planes[5] = glm::vec4(
                viewProjectionMatrix[0][3] + viewProjectionMatrix[0][2],
                viewProjectionMatrix[1][3] + viewProjectionMatrix[1][2],
                viewProjectionMatrix[2][3] + viewProjectionMatrix[2][2],
                viewProjectionMatrix[3][3] + viewProjectionMatrix[3][2]
        );

        // Normalizacja płaszczyzn
        for (int i = 0; i < 6; i++) {
            planes[i] /= glm::length(glm::vec3(planes[i]));
        }
    }

    bool isBoxInFrustum(const CPM_GLM_AABB_NS::AABB& box) const {
        for (int i = 0; i < 6; i++) {
            glm::vec3 positiveVertex = box.getMax();
            glm::vec3 negativeVertex = box.getMin();

            if (planes[i].x >= 0) {
                positiveVertex.x = box.getMin().x;
                negativeVertex.x = box.getMax().x;
            }
            if (planes[i].y >= 0) {
                positiveVertex.y = box.getMin().y;
                negativeVertex.y = box.getMax().y;
            }
            if (planes[i].z >= 0) {
                positiveVertex.z = box.getMin().z;
                negativeVertex.z = box.getMax().z;
            }

            if (glm::dot(glm::vec3(planes[i]), negativeVertex) + planes[i].w > 0) {
                return false;
            }
        }
        return true;
    }
};

#endif