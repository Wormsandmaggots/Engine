#include "windows.h"
#include <iostream>
#include "Debug/Profiler.h"
#include "tracy/TracyOpenGL.hpp"
#include "Text/Text.h"



#include "Engine/Engine.h"
#include "Editor/Editor.h"
#include "Core/AssetManager/AssetManager.h"
#include "ThirdPersonCamera.h"
#include "Physics/ColliderComponent.h"
#include "Physics/CollisionManager.h"
#include "Editor/Gizmos.h"

using namespace SceneManagement;

int main() {

    init();
	//Radek note: don't mind me, just testing
#pragma region TEST
	EditorLayer::Editor editor;
    editor.init(&s.camera);
    CollisionManager cm;
	AudioManager a;
	a.init();
	Sound* sound = a.loadSound("res/content/sounds/Ich will.mp3");
	SceneManager sm;
	sm.loadScene("res/content/maps/exampleScene.yaml");
	//sound->play();
	//! THEE WHO SHALL FIND THIS VOLUME VALUE,
	//! BE AWARE OF CONSEQUENCES STANDING BEHIND ALTERING IT
	sound->setVolume(2.f);
	LOG_INFO("If u hear germans singing, that's a good sing.");
#pragma endregion TEST

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,
                                  (float) s.WINDOW_HEIGHT);
    //ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

	Scene2 scene("scene");
	Entity* entity = new Entity("nanosuit");
    Entity* player = new Entity("player");

	Model* model = new Model("res\\content\\models\\nanosuit\\nanosuit.obj");
    Model* playerModel = new Model("res\\content\\models\\player.obj");
    Model* sphere = new Model("res/content/models/sphere/untitled.obj");

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    cc1->start();
    cc2->start();
	// Model* airplaneModel = new Model("res\\content\\models\\aircraft\\airplane.obj");

	entity->addComponent(model);
    player ->addComponent(playerModel);
    //player->addComponent(playerCamera);
	scene.addEntity(entity);
    scene.addEntity(player);

	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
	Renderer renderer;
    renderer.init();

    float yrotation = 0;
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    sm.getLoadedScenes()[0]->getSceneEntities()[0]->addComponent(cc1);
    sm.getLoadedScenes()[0]->getSceneEntities()[1]->addComponent(cc2);

    sm.getLoadedScenes()[0]->getSceneEntities()[0]->addComponent(sphere);
    sm.getLoadedScenes()[0]->getSceneEntities()[1]->addComponent(sphere);

    cc1->parentTransform = sm.getLoadedScenes()[0]->getSceneEntities()[0]->getTransform();
    cc2->parentTransform = sm.getLoadedScenes()[0]->getSceneEntities()[1]->getTransform();

	// airplane->getTransform()->setPosition(glm::vec3(-5, 0, 1));

	while (!glfwWindowShouldClose(s.window))
	{
        //EditorLayer::Gizmos::Clear();
//        player->getTransform()->setRotation(glm::vec3(0, yrotation, 0));
//        if (glfwGetKey(s.window, GLFW_KEY_S) == GLFW_PRESS)
//        {
//            yrotation+=1;
//        }
//        if (glfwGetKey(s.window, GLFW_KEY_A) == GLFW_PRESS)
//        {
//            yrotation-=1;
//        }
		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;
		processInput(s.window);

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),
		                                        (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
        glm::mat4 view = s.camera.GetViewMatrix();
        //glm::mat4 view = playerCamera->getView();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
        collisionTestShader.use();
        collisionTestShader.setMat4("view", view);
        collisionTestShader.setMat4("projection", projection);
        collisionTestShader.setVec3("color", cc1->color);

        renderer.renderModels(sm.getLoadedScenes()[0]->getSceneEntities());

        imgui_begin();
		editor.draw();
        sm.updateLoadedScenes();
        //scene.update();

        cm.update();

        if(cc1->germansSing)
        {
            //sound->play();
        }
        else
        {
            //sound->stop();
        }

		Profiler::get().markFrame();
        Profiler::get().zoneScope();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //arcadeRenderer->renderText();
        arcadeRenderer->update();
		glfwSwapBuffers(s.window);
		glfwMakeContextCurrent(s.window);
		glfwPollEvents();
	}

    a.end();
    AssetManager::end();
	end();
	return 0;
}
