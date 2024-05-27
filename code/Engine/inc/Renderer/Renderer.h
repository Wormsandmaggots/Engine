#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <vector>
#include "ECS/Entity.h"
#include "Shader.h"
#include "Renderer/Renderable.h"
#include <Debug/Logger.h>
#include "Globals.h"

class Renderer
{
public:
    Renderer(Shader *defaultShader);
    void init();
    void update();
    void end();
    void static Render(Renderable *renderable);
    virtual ~Renderer() = default;
    inline static Shader *defaultShader;
    void updateProjectionAndView(glm::mat4 view, glm::mat4 projection);

    // 1) render do frame buffera
    // 3) frame buffer do post procesow
    // 2) instance render
};

#endif // ENGINE_RENDERER_H
