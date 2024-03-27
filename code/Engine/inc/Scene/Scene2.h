//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_SCENE2_H
#define ENGINE_SCENE2_H


#include <string>
#include "ECS/Entity.h"

class Scene2 {
public:
    explicit Scene2(std::string);
    virtual ~Scene2() = default;

    void addEntity(Entity*);
    void addEntities(std::vector<Entity*>&);
    void update();

    Entity* getRoot() const;
    std::string getName() const;
    const vector<Entity *> &getSceneEntities() const;

private:
    std::string name;
    Entity* root;
    std::vector<Entity*> sceneEntities;
};


#endif //ENGINE_SCENE2_H
