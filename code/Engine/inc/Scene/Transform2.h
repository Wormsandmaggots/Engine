//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_TRANSFORM2_H
#define ENGINE_TRANSFORM2_H

#include "glm/matrix.hpp"
#include "glm/ext/matrix_transform.hpp"

class Transform2 {
public:
    explicit Transform2(
            glm::vec3 localPosition = {0,0,0},
            glm::vec3 localRotation = {0,0,0},
            glm::vec3 localScale = {1,1,1});

    Transform2(const Transform2& t);

    void updateLocalTransform();
    void updateWorldTransform(const glm::mat4 &parentWorldMatrix = {1.f});

    [[nodiscard]] const glm::vec3 &getLocalPosition() const;
    [[nodiscard]] const glm::vec3 &getLocalScale() const;
    [[nodiscard]] const glm::vec3 &getLocalRotation() const;
    [[nodiscard]] const glm::mat4 &getWorldMatrix() const;

    void setPosition(glm::vec3 newPosition);
    void setDirty(bool dirty);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);

private:
    glm::vec3 localPosition {0,0,0};
    glm::vec3 localScale {1,1,1};
    glm::vec3 localRotation {0,0,0};
    glm::mat4 localMatrix {1.f};

    glm::vec3 worldPosition {0,0,0};
    glm::vec3 worldScale {1,1,1};
    glm::vec3 worldRotation {0,0,0};
    glm::mat4 worldMatrix {1.f};

    bool isDirty = true;
};


#endif //ENGINE_TRANSFORM2_H
