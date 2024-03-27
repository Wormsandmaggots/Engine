#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <vector>
#include "ECS/Entity.h"
#include "Shader.h"
class Renderer{
public:
    Renderer(const Shader &shader, const vector<Entity *> &entities);

    virtual ~Renderer() = default;
    void renderModels();
private:
    Shader shader;
    vector<Entity*>entities;
};

#endif //ENGINE_RENDERER_H
