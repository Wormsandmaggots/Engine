#include "Scene/Scene2.h"

void Scene2::addEntity(Entity *entity) {
    if(entity->getParent())
    {
        entity->getParent()->removeChild(entity);
    }

    sceneEntities.push_back(entity);
    //root->addChild(entity);
}

/*void Scene2::update() {
    for (Entity* e : sceneEntities) {
        e->update();
    }
}*/

void Scene2::update() {
    // Get the camera's frustum
    Frustum camFrustum = camera->getFrustum();

    // Iterate over all entities in the scene
    for (Entity* entity : sceneEntities) {
        // Get the model component of the entity
        Model* model = entity->getComponent<Model>();
        if (model != nullptr) {
            // Get the AABB of the model
            AABB modelAABB = model->getAABB();

            // Check if the model's AABB is in the frustum
            model->isVisible = frustumCulling->isOnFrustum(camFrustum, modelAABB);
        }
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
    //root->addChildren(entities);
}

const vector<Entity *> &Scene2::getSceneEntities() const {
    return sceneEntities;
}

void Scene2::removeEntity(Entity *e) {
    sceneEntities.erase(std::remove(sceneEntities.begin(), sceneEntities.end(), e), sceneEntities.end());
}

void Scene2::setName(const string &newName) {
    name = newName;
}


