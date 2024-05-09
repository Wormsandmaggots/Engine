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
    void translate(glm::vec3);
    void scale(glm::vec3);
    void rotate(glm::vec3);

    [[nodiscard]] const glm::vec3 &getLocalPosition() const;
    [[nodiscard]] const glm::vec3 &getLocalScale() const;
    [[nodiscard]] const glm::vec3 &getLocalRotation() const;
    //[[nodiscard]] glm::mat4 &getWorldMatrix();
    [[nodiscard]] const glm::mat4 &getWorldMatrix() const {
        return worldMatrix;
    }
    [[nodiscard]] glm::mat4 &getLocalMatrix();

    void setTransform(glm::mat4 mat);
    void setPosition(glm::vec3 newPosition);
    void setDirty(bool dirty);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);
    void setLocalTransform(glm::mat4 mat);

    glm::vec3 getRight() const {
        return glm::normalize(glm::cross(getUp(), getForward()));
    }

    glm::vec3 getUp() const {
        return glm::vec3(0, 1, 0); // Assuming that the up direction is along the positive y-axis
    }

    glm::vec3 getForward() const {
        return glm::normalize(glm::vec3(getWorldMatrix()[2])); // Assuming that the forward direction is the z-axis
    }


private:
    glm::vec3 localPosition=glm::vec3(0.f);
    glm::vec3 localScale=glm::vec3(1.f);
    glm::vec3 localRotation=glm::vec3(0.f);
    glm::mat4 localMatrix = glm::mat4(1.f);

    glm::vec3 worldPosition;
    glm::vec3 worldScale;
    glm::vec3 worldRotation;
    glm::mat4 worldMatrix = glm::mat4(1.f);

    bool isDirty = true;

    void updateLocalTransform();
};


#endif //ENGINE_TRANSFORM2_H
