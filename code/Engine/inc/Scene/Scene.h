//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_SCENE2_H
#define ENGINE_SCENE2_H


#include <string>
#include "ECS/Entity.h"
#include "Core/AssetManager/Asset.h"

class Scene {
public:
    explicit Scene(std::string);
    ~Scene() = default;

    void addEntity(Entity*);
    void addEntities(std::vector<Entity*>&);
    void update();
    void removeEntity(Entity*);

    void setName(const std::string&);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const vector<Entity *> &getSceneEntities() const;

private:
    std::string name;
    std::vector<Entity*> sceneEntities;
};


#endif //ENGINE_SCENE2_H
