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
#include "Scene/Transform.h"
#include "ThirdPersonCamera.h"

class Component;

class Entity {
public:
    static void ResetCounter();
    explicit Entity(const std::string &name = "");
    Entity(const Entity&);
    virtual ~Entity();

    void update();
    void updateTransform();
    void setDirtyTree();
    void addComponent(Component*);
    void addChild(Entity*);
    void addChildren(std::vector<Entity*>&);
    void removeChild(Entity*);
    void removeComponent(Component*);
    void removeComponent(int);

    [[nodiscard]] Transform* getTransform() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Entity* getParent() const;
    [[nodiscard]] std::vector<Entity*> getChildren() const;
    [[nodiscard]] std::vector<Component*> getComponents() const;
    [[nodiscard]] int getId() const;

    void setTransform(const Transform&);
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
    
    void toYaml(YAML::Emitter&);

    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        T* newComponent = new T(std::forward<Args>(args)...);
        newComponent->setParent(this);
        components.push_back(newComponent);}

    void drawEditor();


private:
    static int EntityCounter;
    std::string name = "Untitled" + std::to_string(EntityCounter);
    Transform* transform = new Transform;
    int id = 0;
    Entity* parent = nullptr;
    std::vector<Entity*> children;
    std::vector<Component*> components;
};


#endif //ENGINE_ENTITY_H
