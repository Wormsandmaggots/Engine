//
// Created by Radek on 26.05.2024.
//

#ifndef ENGINE_SCRIPTABLEENTITY_H
#define ENGINE_SCRIPTABLEENTITY_H

#include "Entity.h"
#include "Physics/ColliderComponent.h"
#include "Debug/Logger.h"

class ScriptableEntity : public Entity
{
public:
    explicit ScriptableEntity(const string &name);
    explicit ScriptableEntity(const Entity &entity);

    void update() override;
    void addComponent(Component *) override;
    virtual void onTriggerEnter(ColliderComponent *) {};
    virtual void onTriggerStay(ColliderComponent *) {};
    virtual void onTriggerExit(ColliderComponent *) {};
};

#endif // ENGINE_SCRIPTABLEENTITY_H
