#include "windows.h"
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
#include "Input/DebugInput.h"
#include "HUD/ProgressBar.h"
#include "HUD/BackgroundImage.h"
#include "Renderer/MaterialAsset.h"
using namespace SceneManagement;

int main() {
    init();
	EditorLayer::Editor editor;
    editor.init(&s.camera);
    CollisionManager cm;
	AudioManager a;
	a.init();
	Sound* sound = a.loadSound("res/content/sounds/Ich will.mp3");
	SceneManager sm;

	sm.loadScene("res/content/maps/test.yaml");

	sound->setVolume(2.f);

    //HID - test
    //TODO: Kuba: Czy to może tutaj zostać?
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    //HID - test
    DebugInput debugInput;
    //HID - test

#pragma endregion TEST

	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
	Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
	Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");

    //TODO: Kuba: Czy to może tutaj zostać?

    //HUD
    ProgressBar progressBar("res/content/shaders/vertex_2d.glsl", "res/content/shaders/progress_bar_fragment.glsl", "res/content/textures/bar.png", 100.0f);
    BackgroundImage backgroundImage("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/nodes.png");
    Image image("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/hud_back.png");


    Renderer renderer(&shader);

	renderer.addShader(&collisionTestShader);
	renderer.addShader(&shaderText);

	renderer.init();

    Model* player = new Model("res/content/models/player/character_base.obj");


    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,
                                  (float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    //MaterialAsset material("C:\\gierki\\pbl\\Engine\\res\\content\\materials\\material.json");
    //MaterialAsset material("res/content/materials/material.json");

    cc1->start();
    cc2->start();

	//sm.getLoadedScenes()[0]->addEntity(new Entity("player"));
	//sm.getLoadedScenes()[0]->getSceneEntities()[2]->addComponent(player);
    //player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    while (!glfwWindowShouldClose(s.window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = s.camera.GetViewMatrix();

		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
		//glm::mat4 view = playerCamera->getView();

        imgui_begin();
        editor.draw();
		
		renderer.updateProjectionAndView(projection, view);

        sm.updateLoadedScenes();
        //scene.update();
        cm.update();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //arcadeRenderer->renderText();




        //TODO: Kuba: Muszę poprawić renderowanie textu u siebie
        //hud
        counterRenderer->renderAndUpdateCounter(shaderText, s.deltaTime, 300, 160, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);//if rectangle display queue broken, uncomment glDisabe/glEnable
        glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(s.WINDOW_WIDTH), 0.0f, static_cast<float>(s.WINDOW_HEIGHT));
        shader.setMat4("projection", orthoProjection);

        progressBar.update(s.deltaTime);
        progressBar.renderBar();

        image.render();
        backgroundImage.render();
        backgroundImage.update(s.deltaTime);

        sm.updateLoadedScenes();

        cm.update();
        arcadeRenderer->update();
        update();
	}
    a.end();

	end();
	return 0;
}
