//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_TRANSFORM2_H
#define ENGINE_TRANSFORM2_H

#include "glm/matrix.hpp"
#include "glm/ext/matrix_transform.hpp"

class Transform2 {
public:
    Transform2();
    Transform2(glm::vec3 localPosition,
               glm::vec3 localRotation,
               glm::vec3 localScale);
    Transform2(const Transform2 &t);

    virtual ~Transform2() = default;

    void updateWorldTransform(const glm::mat4 &parentWorldMatrix = glm::mat4(1));

    [[nodiscard]] const glm::vec3 &getLocalPosition() const;
    [[nodiscard]] const glm::vec3 &getLocalScale() const;
    [[nodiscard]] const glm::vec3 &getLocalRotation() const;
    [[nodiscard]] glm::mat4 &getWorldMatrix();
    [[nodiscard]] glm::mat4 &getLocalMatrix();

    void setTransform(glm::mat4 mat);
    void setPosition(glm::vec3 newPosition);
    void setDirty(bool dirty);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);
    void setLocalTransform(glm::mat4 mat);

private:
    glm::vec3 localPosition=glm::vec3(0);
    glm::vec3 localScale=glm::vec3(1);
    glm::vec3 localRotation=glm::vec3(0);
    glm::mat4 localMatrix = glm::mat4(1.f);

    glm::vec3 worldPosition;
    glm::vec3 worldScale;
    glm::vec3 worldRotation;
    glm::mat4 worldMatrix = glm::mat4(1.f);

    bool isDirty = true;

    void updateLocalTransform();
};


#endif //ENGINE_TRANSFORM2_H
