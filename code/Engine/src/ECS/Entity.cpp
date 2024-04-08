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

Entity::Entity(const string &name) {
    if(!name.empty())
    {
        this->name = name;
    }

    id = EntityCounter;
    EntityCounter++;
}

Transform2 *Entity::getTransform() const {
    return transform;
}

void Entity::addComponent(Component *component) {
    components.push_back(component);
    component->setParent(this);
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
    id = EntityCounter;
    EntityCounter++;
    parent = e.parent;
    children = e.children;
    components = e.components;
}

void Entity::setParent(Entity &newParent) {
    if(parent)
    {
        parent->removeChild(this);
    }

    parent = &newParent;

    parent->children.push_back(this);
}

//adds child to children of Entity and sets child's parent
void Entity::addChild(Entity *child) {
    if(child->parent == this) return;

    if(child->parent)
    {
        child->parent->removeChild(child);
    }

    children.push_back(child);

    child->parent = this;
}

std::string Entity::getName() const {
    return name;
}

std::vector<Component *> Entity::getComponents() const {
    return components;
}

std::vector<Entity *> Entity::getChildren() const {
    return children;
}

Entity *Entity::getParent() const {
    return parent;
}

void Entity::addChildren(std::vector<Entity *>& entities) {
    children.insert(children.end(), entities.begin(), entities.end());

    for (Entity* e : entities) {
        e->setParent(*this);
    }
}

int Entity::getId() const {
    return id;
}

Entity::~Entity() {
    for (Entity* e : children) {
        delete e;
    }

    delete transform;
}

void Entity::removeChild(Entity *e) {
    children.erase(std::remove(children.begin(), children.end(), e), children.end());
    e->parent = nullptr;
}
