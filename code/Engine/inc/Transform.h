#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <cmath>
#include "Model.h"

class Transform
{
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 localTransform;
    glm::mat4 worldTransform;

    bool hasModel = false;

    int windowWidth = 1000;
    int windowHeight = 1000;

    Model* myModel;
    Transform* parent;
    std::vector<Transform*> children;

    Transform()
    {
        this->position = glm::vec3(0, 0, 0);
        this->rotation = glm::vec3(0,0,0);
        this->scale = glm::vec3(1);
        this->myModel = nullptr;
        this->worldTransform = glm::mat4(1.0f);
        this->localTransform = glm::mat4(1.0f);
    }

    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, Model* myModel = nullptr)
    {

        this->position = position;
        this->rotation = rotation;
        this->scale = scale;
        this->myModel = myModel;
        this->worldTransform = glm::mat4(1.0f);
        this->localTransform = glm::mat4(1.0f);
    }

    void setPosition(glm::vec3 newPosition)
    {
        this->position = newPosition;
    }

    void setRotation(glm::vec3 newRotation)
    {
        this->rotation = newRotation;
    }

    void addChild(Transform* child)
    {
        child->parent = this;
        children.push_back(child);
    }

    void setScale(glm::vec3 scale)
    {
        this->scale = scale;
    }

    void updateLocalTransform()
    {
        glm::mat4 transform = glm::mat4(1.0f);

        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, rotation.y, glm::vec3(0.0, 1.0, 0.0));
        transform = glm::rotate(transform, rotation.x, glm::vec3(1.0, 0.0, 0.0));
        transform = glm::rotate(transform, rotation.z, glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, scale);
        localTransform = transform;
    }

    void updateWorldTransform(glm::mat4 modelLok, Shader shader)
    {
        this->updateLocalTransform();
        this->worldTransform = modelLok * localTransform;

        for (Transform* child : children) {
            child->updateWorldTransform(this->worldTransform, shader);
        }

        shader.setMat4("model", this->worldTransform);
    }
};