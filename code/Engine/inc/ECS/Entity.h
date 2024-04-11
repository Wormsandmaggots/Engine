//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "Model.h"
#include <string>
#include <vector>
#include "Scene/Transform2.h"
#include "ThirdPersonCamera.h"

class Component;

class Entity {
public:
    explicit Entity(const std::string &name = "");
    Entity(const Entity&);
    virtual ~Entity();

    void update();
    void addComponent(Component*);
    void addChild(Entity*);
    void addChildren(std::vector<Entity*>&);
    void removeChild(Entity*);

    [[nodiscard]] Transform2* getTransform() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Entity* getParent() const;
    [[nodiscard]] std::vector<Entity*> getChildren() const;
    [[nodiscard]] std::vector<Component*> getComponents() const;
    [[nodiscard]] int getId() const;

    void setTransform(const Transform2&);
    void setName(const std::string&);
    void setParent(Entity&);
    // Template function to get a component of a specific type
    template<typename T>
    T* getComponent() const {
        // Loop through components to find the Model component
        for (Component* component : components) {
            // Attempt to cast the component pointer to Model type
            T* tComponent = dynamic_cast<T*>(component);
            if (tComponent != nullptr) {
                // Found the Model component, return it
                return tComponent;
            }
        }
        // If not found, return nullptr
        return nullptr;
    }
    //T* getComponent() const;

private:
    static int EntityCounter;
    std::string name = "Untitled" + std::to_string(EntityCounter);
    Transform2* transform = new Transform2;
    int id = 0;
    Entity* parent = nullptr;
    std::vector<Entity*> children;
    std::vector<Component*> components;
};


#endif //ENGINE_ENTITY_H
