#ifndef ENGINE_RENDERABLE_H
#define ENGINE_RENDERABLE_H

class Renderable
{

public:
    virtual Shader* getShader() = 0;
    virtual Transform2* getTransform() = 0;
    virtual void Draw(Shader* shader)=0;
};

#endif //ENGINE_RENDERABLE_H
