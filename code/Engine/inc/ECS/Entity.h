//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_ENTITY_H
#define ENGINE_ENTITY_H

#include <string>
#include <vector>
#include "Scene/Transform2.h"
#include "Transform_old.h"

class Component;

class Entity {
public:
    explicit Entity(const std::string &name = "");
    Entity(const Entity&);
    virtual ~Entity() = default;

    void update();
    void addComponent(Component*);
    void addChild(Entity*);

    [[nodiscard]] Transform2* getTransform() const;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Entity* getParent() const;
    [[nodiscard]] std::vector<Entity*> getChildren() const;
    [[nodiscard]] std::vector<Component*> getComponents() const;

    void setTransform(const Transform2&);
    void setName(const std::string&);
    void setParent(Entity&);

private:
    static int EntityCounter;
    std::string name = "Untitled" + std::to_string(EntityCounter);
    Transform2* transform = new Transform2;
    Entity* parent = nullptr;
    std::vector<Entity*> children;
    std::vector<Component*> components;
};


#endif //ENGINE_ENTITY_H
