//
// Created by Radek on 24.03.2024.
//

#include "Scene/Scene2.h"

void Scene2::addEntity(Entity *entity) {
    sceneEntities.push_back(entity);
}

void Scene2::update() {
    for (Entity* e : sceneEntities) {
        e->update();
    }
}

Scene2::Scene2(std::string sceneName) {
    name = sceneName;
}

std::string Scene2::getName() const {
    return name;
}

void Scene2::addEntities(std::vector<Entity *>& entities) {
    sceneEntities.insert(sceneEntities.end(), entities.begin(), entities.end());
}


