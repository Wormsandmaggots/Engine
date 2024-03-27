//
// Created by Radek on 23.03.2024.
//

#include "ECS/Entity.h"
#include "ECS/Component.h"

int Entity::EntityCounter = 0;

//!works like DFS when it comes to updating queue
void Entity::update() {
    //firstly update transform
    if(parent != nullptr)
    {
        transform->updateWorldTransform(parent->transform->getWorldMatrix());
    }
    else
    {
        transform->updateWorldTransform();
    }

    //then update components because they can change transform
    for (Component* c : components) {
        c->update();
    }

    //then update children
    for (Entity* child : children) {
        child->update();
    }
}

Entity::Entity(const string &name) : name(name){}

Transform2 *Entity::getTransform() const {
    return transform;
}

void Entity::addComponent(Component *component) {
    components.push_back(component);
}

void Entity::setTransform(const Transform2 &newTransform) {
    delete transform;

    transform = new Transform2(newTransform);
}

void Entity::setName(const string &newName) {
    name = newName;
}

Entity::Entity(const Entity &e) {
    setName(e.name + "1");
    setTransform(*e.getTransform());
    parent = e.parent;
    children = e.children;
    components = e.components;
}

void Entity::setParent(Entity &newParent) {
    if(parent != nullptr) delete parent;

    parent = &newParent;
}

//adds child to children of Entity and sets child's parent
void Entity::addChild(Entity *child) {
    children.push_back(child);

    child->parent = this;
}

std::string Entity::getName() const {
    return name;
}

std::vector<Component *> Entity::getComponents() const {
    return components;
}
