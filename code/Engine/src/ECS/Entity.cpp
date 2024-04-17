#include "ECS/Entity.h"
#include "ECS/Component.h"
#include "yaml-cpp/yaml.h"
#include "imgui.h"


int Entity::EntityCounter = 0;

//!works like DFS when it comes to updating queue
void Entity::update() {
    //firstly update transform
    updateTransform();

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

void Entity::toYaml(YAML::Emitter &emitter) {

    emitter << YAML::BeginMap << YAML::Key << "Name" << YAML::Anchor(to_string(id)) << YAML::Value << name;

    if(parent != nullptr)
    {
        emitter << YAML::Key << "Parent" << YAML::Alias(to_string(parent->id));
    }

    float pos[3] = {transform->getLocalPosition().x,
                    transform->getLocalPosition().y,
                    transform->getLocalPosition().z};

    float rot[3] = {transform->getLocalRotation().x,
                    transform->getLocalRotation().y,
                    transform->getLocalRotation().z};

    float scale[3] = {transform->getLocalScale().x,
                    transform->getLocalScale().y,
                    transform->getLocalScale().z};

    emitter << YAML::Key << "Transform" << YAML::BeginMap;
    emitter << YAML::Key << "pos" << YAML::Flow << YAML::BeginSeq << pos[0] << pos[1] << pos[2] << YAML::EndSeq;
    emitter << YAML::Key << "rot" << YAML::Flow << YAML::BeginSeq << rot[0] << rot[1] << rot[2] << YAML::EndSeq;
    emitter << YAML::Key << "scale" << YAML::Flow << YAML::BeginSeq << scale[0] << scale[1] << scale[2] << YAML::EndSeq;
    emitter << YAML::EndMap;

    emitter << YAML::Key << "Components" << YAML::BeginSeq;

    for(Component* c : components)
    {
        emitter << YAML::BeginMap;
        c->convertToYaml(emitter);
        emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;

    emitter << YAML::EndMap;

    for (Entity* e : children) {
        e->toYaml(emitter);
    }
}

void Entity::drawEditor() {
    int i = 0;
    for (Component* c : components) {
        c->drawEditor();
        ImGui::Button("Remove component");
        if(ImGui::IsItemClicked())
        {
            removeComponent(i);
        }

        ImGui::Separator();
        i++;
    }
}

void Entity::removeComponent(Component *c) {
    components.erase(std::remove(components.begin(), components.end(), c), components.end());

    delete c;
}


void Entity::updateTransform() {
    if(parent != nullptr)
    {
        transform->updateWorldTransform(parent->transform->getWorldMatrix());
    }
    else
    {
        transform->updateWorldTransform();
    }
}

void Entity::setDirtyTree() {
    for (Entity* e : children) {
        e->transform->setDirty(true);
        e->setDirtyTree();
    }
}

void Entity::ResetCounter() {
    EntityCounter = 0;
}

void Entity::removeComponent(int i) {
    Component* c = components.at(i);
    components.erase(components.begin() + i);
    delete c;
}

template<>
Model* Entity::getComponent<Model>() const {
    // Loop through components to find the Model component
    for (Component* component : components) {
        // Attempt to cast the component pointer to Model type
        Model* modelComponent = dynamic_cast<Model*>(component);
        if (modelComponent != nullptr) {
            // Found the Model component, return it
            return modelComponent;
        }
    }
    // If not found, return nullptr
    return nullptr;
}


