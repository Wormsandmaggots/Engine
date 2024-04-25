//
// Created by Radek on 24.03.2024.
//

#ifndef ENGINE_SCENE2_H
#define ENGINE_SCENE2_H


#include <string>
#include "ECS/Entity.h"
#include "Core/AssetManager/Asset.h"

class Scene2 {
public:
    explicit Scene2(std::string);
    ~Scene2() = default;

    void addEntity(Entity*);
    void addEntities(std::vector<Entity*>&);
    void update();
    void removeEntity(Entity*);

    void setName(const std::string&);

    [[nodiscard]] std::string getName() const;
    [[nodiscard]] const vector<Entity *> &getSceneEntities() const;

    //for frustum culling TODO: refactor to fit convention

    std::vector<Renderable*> getAllEntities() const {
        std::vector<Renderable*> renderables;
        for (Entity* entity : sceneEntities) {
            Renderable* renderable = dynamic_cast<Renderable*>(entity);
            if (renderable) {
                renderables.push_back(renderable);
            }
        }
        return renderables;
    }

private:
    std::string name;
    std::vector<Entity*> sceneEntities;
};


#endif //ENGINE_SCENE2_H
