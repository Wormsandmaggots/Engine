#ifndef ENGINE_RENDERABLE_H
#define ENGINE_RENDERABLE_H

class Renderable
{
public:
    virtual void Draw(Shader& shader) = 0; // Pure virtual function
};

#endif //ENGINE_RENDERABLE_H