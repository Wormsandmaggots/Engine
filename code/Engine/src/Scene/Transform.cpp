//
// Created by Radek on 24.03.2024.
//

#include "Scene/Transform.h"

Transform::Transform(glm::vec3 localPosition,
        glm::vec3 localRotation,
        glm::vec3 localScale) {
    this->localPosition = localPosition;
    this->localRotation = localRotation;
    this->localScale = localScale;
    isDirty = true;
}
Transform::Transform(){
    this->localPosition = glm::vec3(0,0,0);
    this->localRotation = glm::vec3(0,0,0);
    this->localScale = glm::vec3(1,1,1);
    isDirty = true;
}

Transform::Transform(const Transform &t) {
    localPosition = t.localPosition;
    localRotation = t.localRotation;
    localScale = t.localScale;
    isDirty = true;
}

void Transform::setPosition(glm::vec3 newPosition) {
    localPosition = newPosition;
    isDirty = true;
}

void Transform::setDirty(bool dirty) {
    this->isDirty = dirty;
}

void Transform::setRotation(glm::vec3 newRotation) {
    localRotation = newRotation;
    isDirty = true;
}

void Transform::setScale(glm::vec3 newScale) {
    localScale = newScale;
    isDirty = true;
}

void Transform::updateLocalTransform() {
    glm::mat4 mat = glm::mat4(1.0f);

    mat = glm::translate(mat, localPosition);
    mat = glm::rotate(mat, glm::radians(localRotation.y), glm::vec3(0.0, 1.0, 0.0));
    mat = glm::rotate(mat, glm::radians(localRotation.x), glm::vec3(1.0, 0.0, 0.0));
    mat = glm::rotate(mat, glm::radians(localRotation.z), glm::vec3(0.0, 0.0, 1.0));
    mat = glm::scale(mat, localScale);

    localMatrix = mat;
}

void Transform::setLocalTransform(glm::mat4 mat) {
    localMatrix = mat;
    //ImGuizmo::DecomposeMatrixToComponents(localMatrix, matrixTranslation, matrixRotation, matrixScale);
    isDirty = true;
}

void Transform::updateWorldTransform(const glm::mat4 &parentWorldMatrix) {
    if (isDirty) {
        updateLocalTransform();
        worldMatrix = localMatrix * parentWorldMatrix;
        isDirty = false;
    }
}

const glm::vec3 &Transform::getLocalPosition() const {
    return localPosition;
}

const glm::vec3 &Transform::getLocalScale() const {
    return localScale;
}

const glm::vec3 &Transform::getLocalRotation() const {
    return localRotation;
}

glm::mat4 &Transform::getWorldMatrix()  {
    return worldMatrix;
}

void Transform::setTransform(glm::mat4 mat) {
    this->worldMatrix = mat;
    isDirty = true;
}

glm::mat4 &Transform::getLocalMatrix() {
    return localMatrix;
}

void Transform::translate(glm::vec3 t) {
    localPosition += t;
    isDirty = true;
}

void Transform::scale(glm::vec3 s) {
    localScale += s;
    isDirty = true;
}

void Transform::rotate(glm::vec3 r) {
    localRotation += r;
    isDirty = true;
}

