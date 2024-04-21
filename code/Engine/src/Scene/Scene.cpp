//
// Created by Radek on 24.03.2024.
//

#include "Scene/Scene.h"

void Scene::addEntity(Entity *entity) {
    if(entity->getParent())
    {
        entity->getParent()->removeChild(entity);
    }

    sceneEntities.push_back(entity);
    //root->addChild(entity);
}

void Scene::update() {
    for (Entity* e : sceneEntities) {
        e->update();
    }
}

Scene::Scene(std::string sceneName) {
    name = sceneName;
}

std::string Scene::getName() const {
    return name;
}

void Scene::addEntities(std::vector<Entity *>& entities) {
    sceneEntities.insert(sceneEntities.end(), entities.begin(), entities.end());
    //root->addChildren(entities);
}

const vector<Entity *> &Scene::getSceneEntities() const {
    return sceneEntities;
}

void Scene::removeEntity(Entity *e) {
    sceneEntities.erase(std::remove(sceneEntities.begin(), sceneEntities.end(), e), sceneEntities.end());
}

void Scene::setName(const string &newName) {
    name = newName;
}


