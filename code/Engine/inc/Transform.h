#ifndef OPENGLGP_TRANSFORM_H
#define OPENGLGP_TRANSFORM_H

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Model.h"
#include <vector>
#include <string>
#include <cmath>

using namespace glm;

class Transform
{
public:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotateModel;
    glm::mat4* model;
    glm::mat4 localTransform;
    Model* m;
    Shader* shader;
    bool shouldTexture;

    Transform* parent;
    std::vector<Transform*> children;

    Transform(Model* m = nullptr,
              glm::vec3 position = vec3(0,0,0),
              glm::vec3 rotateModel = vec3(0,0,0),
              glm::vec3 scale = vec3(1,1,1),
              bool t = false, Shader* s = nullptr)
    {
        this->m = m;
        this->position = position;
        this->rotateModel = rotateModel;
        this->scale = scale;
        shader = s;

        shouldTexture = t;
    }

    void setModel(Model* m)
    {
        this->m = m;
    }

    void setModel(glm::mat4* model)
    {
        this->model = model;
    }

    void setPosition(glm::vec3 newPosition)
    {
        position = newPosition;
        if(parent == nullptr) return;

//        position.x += parent->position.x;
//        position.y += parent->position.y;
//        position.z += parent->position.z;
    }

    void setRotation(glm::vec3 newRotation)
    {
        rotateModel = newRotation;
    }

    glm::mat4* getModel()
    {
        return model;
    }

    void addChild(Transform *child)
    {
        children.push_back(child);
        child->parent = this;
    }

    void updateLocalTransform()
    {
        glm::mat4 transform = glm::mat4(1.0f);

        transform = glm::translate(transform, position);
        transform = glm::rotate(transform, rotateModel.y, glm::vec3(0.0, 1.0, 0.0));
        transform = glm::rotate(transform, rotateModel.x, glm::vec3(1.0, 0.0, 0.0));
        transform = glm::rotate(transform, rotateModel.z, glm::vec3(0.0, 0.0, 1.0));
        transform = glm::scale(transform, scale);
        localTransform = transform;
    }

    virtual void updateWorldTransform(glm::mat4* modelLok, Shader *sh, bool updateLocal = true)
    {
        if(updateLocal)
        {
            this->updateLocalTransform();
            this->model = new glm::mat4(*modelLok * localTransform);
        }

        for (Transform* child : children) {
            child->updateWorldTransform(this->model, shader == nullptr ? sh : shader, updateLocal);
        }

        if(sh != nullptr)
        {
            sh->use();
            sh->setMat4("model", *this->model);
        }

        if(m != nullptr && (sh != nullptr || shader != nullptr))
        {
            if(shader != nullptr)
            {
                shader->use();
                shader->setMat4("model", *this->model);
            }

            m->Draw(shader == nullptr ? *sh : *shader);
        }

    }
};


#endif //OPENGLGP_TRANSFORM_H
