#include "Renderer/Renderer.h"
#include <Renderer/MaterialAsset.h>

Renderer::Renderer(Shader* shader) {
	globalShaders.push_back(shader);
	defaultShader = shader;
 }


void Renderer::init()
{
	//    stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}



void Renderer::update()
{
	
}

void Renderer::end()
{

}


void Renderer::Render(Renderable *renderable)
{

	if (renderable->getShader() == nullptr) {
		defaultShader->use();
		defaultShader->setMat4("model", renderable->getTransform()->getLocalMatrix());
        defaultShader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(renderable->getTransform()->getWorldMatrix()))));
        renderable->Draw(defaultShader);
	}
	else {
		renderable->getShader()->use();
		renderable->getShader()->setMat4("model", renderable->getTransform()->getLocalMatrix());
        renderable->getShader()->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(renderable->getTransform()->getWorldMatrix()))));
        renderable->Draw(renderable->getShader());
	}
	
}

void Renderer::updateProjectionAndView(glm::mat4 projection, glm::mat4 view, glm::vec3 camPos)
{
	for (Shader* shader : globalShaders) {
		shader->use();		
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
        shader->setVec3("camPos", camPos);
	}
}
