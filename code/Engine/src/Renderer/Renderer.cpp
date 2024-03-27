#include "Renderer/Renderer.h"
#include "Renderer/Renderable.h"

Renderer::Renderer(const Shader &shader, const vector<Entity *> &entities) : shader(shader), entities(entities) {}

void Renderer::renderModels() {
    for(auto entity:entities){
        for(auto component:entity->getComponents()){
            Renderable* renderable = dynamic_cast<Renderable*>(component);
            if(renderable) {
                entity->getTransform()->updateWorldTransform();
                shader.setMat4("model", entity->getTransform()->getWorldMatrix());
                renderable->Draw(shader);
            }
        }
    }
}
