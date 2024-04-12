#include "Renderer/Renderer.h"
#include "Renderer/Renderable.h"

Renderer::Renderer() {
 }


void Renderer::init()
{
	shader = new Shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");

	//    stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::end()
{

}

void Renderer::Render(Renderable *renderable)
{
	renderable->Draw(shader);
}