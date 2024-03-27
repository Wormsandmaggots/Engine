#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <vector>
#include "Model.h"
class Renderer{
public:
    Renderer();
    void renderModels();
    std::vector<Model*> models;


};

#endif //ENGINE_RENDERER_H
