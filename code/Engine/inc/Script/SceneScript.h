#ifndef ENGINE_SCENESCRIPT_H
#define ENGINE_SCENESCRIPT_H

#include "glm/fwd.hpp"
#include "DependencyContainer.h"

class SceneScript{
protected:
    //DependencyContainer& dependencies;
public:
    SceneScript() = default;

    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update() = 0;
    virtual void onDestroy() = 0;
    virtual ~SceneScript() = default;

};

#endif
