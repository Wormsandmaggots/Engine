#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <vector>
#include "ECS/Entity.h"
#include "Shader.h"
class Renderer{
public:
    Renderer();
    void init();
    void end();
    void static Render(Renderable* renderable);
    virtual ~Renderer() = default;
    inline static Shader* shader;

    //1) statyczna metoda render() if ma mesh to ten shader jak nie to deafultowy shader;
    //3) frame buffer do post procesow 
    //2) instance render

};

#endif //ENGINE_RENDERER_H
