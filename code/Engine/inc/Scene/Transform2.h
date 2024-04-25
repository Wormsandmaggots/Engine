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
    [[nodiscard]] glm::mat4 &getWorldMatrix();
    [[nodiscard]] glm::mat4 &getLocalMatrix();

    //for frustum culling TODO: refactor to fit convention
    glm::vec3 getGlobalScale() const {
        // Compute the global scale by transforming the local scale with the world matrix
        glm::vec4 localScale4(localScale, 1.0f);
        glm::vec4 globalScale4 = worldMatrix * localScale4;
        return glm::vec3(globalScale4);
    }

    glm::mat4 getModelMatrix() const {
        // The model matrix is the world matrix in this case
        return worldMatrix;
    }

    glm::vec3 getRight() const {
        return glm::vec3(localMatrix[0]);
    }

    glm::vec3 getUp() const {
        return glm::vec3(localMatrix[1]);
    }

    glm::vec3 getForward() const {
        return glm::vec3(localMatrix[2]);
    }

    glm::vec3 getPosition() const {
        return localPosition;
    }

    glm::vec3 getScale() const {
        return localScale;
    }

    void setTransform(glm::mat4 mat);
    void setPosition(glm::vec3 newPosition);
    void setDirty(bool dirty);
    void setRotation(glm::vec3 newRotation);
    void setScale(glm::vec3 newScale);
    void setLocalTransform(glm::mat4 mat);

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
