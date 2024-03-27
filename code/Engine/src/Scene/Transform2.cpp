//
// Created by Radek on 24.03.2024.
//

#include "Engine/inc/Scene/Transform2.h"

Transform2::Transform2(glm::vec3 localPosition,
        glm::vec3 localRotation,
        glm::vec3 localScale) {
    this->localPosition = localPosition;
    this->localRotation = localRotation;
    this->localScale = localScale;
    isDirty = true;
}
Transform2::Transform2(){
    this->localPosition = glm::vec3(0,0,0);
    this->localRotation = glm::vec3(0,0,0);
    this->localScale = glm::vec3(1,1,1);
    isDirty = true;
}

Transform2::Transform2(const Transform2 &t) {
    localPosition = t.localPosition;
    localRotation = t.localRotation;
    localScale = t.localScale;
    isDirty = true;
}

void Transform2::setPosition(glm::vec3 newPosition) {
    localPosition = newPosition;
    isDirty = true;
}

void Transform2::setDirty(bool dirty) {
    this->isDirty = dirty;
}

void Transform2::setRotation(glm::vec3 newRotation) {
    localRotation = newRotation;
    isDirty = true;
}

void Transform2::setScale(glm::vec3 newScale) {
    localScale = newScale;
    isDirty = true;
}

void Transform2::updateLocalTransform() {
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, localPosition);
    mat = glm::rotate(mat, localRotation.y, glm::vec3(0.0, 1.0, 0.0));
    mat = glm::rotate(mat, localRotation.x, glm::vec3(1.0, 0.0, 0.0));
    mat = glm::rotate(mat, localRotation.z, glm::vec3(0.0, 0.0, 1.0));
    mat = glm::scale(mat, localScale);
    localMatrix = mat;
}

void Transform2::updateWorldTransform(const glm::mat4 &parentWorldMatrix) {
    if (isDirty) {
        updateLocalTransform();
        worldMatrix = parentWorldMatrix * localMatrix;
        isDirty = false;
    }
}

const glm::vec3 &Transform2::getLocalPosition() const {
    return localPosition;
}

const glm::vec3 &Transform2::getLocalScale() const {
    return localScale;
}

const glm::vec3 &Transform2::getLocalRotation() const {
    return localRotation;
}

const glm::mat4 &Transform2::getWorldMatrix() const {
    return worldMatrix;
}

