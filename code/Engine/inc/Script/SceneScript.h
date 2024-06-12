#ifndef ENGINE_SCENESCRIPT_H
#define ENGINE_SCENESCRIPT_H

#include "glm/fwd.hpp"

class SceneScript{
public:
    virtual void awake() = 0;
    virtual void start() = 0;
    virtual void update(const glm::mat4& projection, const glm::mat4& view) = 0;
    virtual void onDestroy() = 0;
    virtual ~SceneScript() = default;

};

#endif
