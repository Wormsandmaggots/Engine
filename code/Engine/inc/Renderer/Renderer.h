#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include <vector>
#include "ECS/Entity.h"
#include "Shader.h"
#include "Renderer/Renderable.h"
#include "Renderer/Material.h"
#include <Debug/Logger.h>



class Renderer{
public:
    Renderer(Shader* defaultShader);
    void init();
    void update();
    void end();
    void addShader(Shader* shader);
    void static Render(Renderable* renderable);
    virtual ~Renderer() = default;
    inline static Shader* defaultShader;
    std::vector<Shader*> shaders;
    void updateProjectionAndView(glm::mat4 view, glm::mat4 projection);
   
    //3) frame buffer do post procesow 
    //2) instance render

};

#endif //ENGINE_RENDERER_H
