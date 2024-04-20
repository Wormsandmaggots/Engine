#ifndef ENGINE_RENDERABLE_H
#define ENGINE_RENDERABLE_H

#include "Shader.h"
#include "Scene/Transform2.h"

class Renderable
{

public:
    virtual Shader* getShader() = 0;
    virtual Transform2* getTransform() = 0;
    virtual void Draw(Shader* shader)=0;
};

#endif //ENGINE_RENDERABLE_H
