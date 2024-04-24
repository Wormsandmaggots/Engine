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
#include "Input/DebugInput.h"
#include "HUD/ProgressBar.h"
#include "HUD/BackgroundImage.h"

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
	sm.loadScene("res/content/maps/exampleScene.yaml");
	sound->setVolume(2.f);


    //HID - test
    //TODO: Kuba: Czy to może tutaj zostać?
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    //HID - test
    DebugInput debugInput;
    //HID - test

	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
	Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
	Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    //TODO: Kuba: Czy to może tutaj zostać?

    //HUD
    ProgressBar progressBar("res/content/shaders/vertex_2d.glsl", "res/content/shaders/progress_bar_fragment.glsl", "res/content/textures/bar.png", 100.0f);
    BackgroundImage backgroundImage("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/nodes.png");
    Image image("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/hud_back.png");


    Renderer renderer(&shader);

	renderer.addShader(&collisionTestShader);
	renderer.addShader(&shaderText);
    renderer.addShader(&shaderPbr);
    renderer.addShader(&shaderCel);

	renderer.init();

    Model* club = new Model("res\\content\\models\\club2\\club2.obj", &shaderPbr);
	Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &collisionTestShader);
	//Model* player = new Model("res\\content\\models\\player\\character_base.obj", &shaderPbr);
    Model* player = new Model("res\\content\\models\\nanosuit\\nanosuit.obj", &shaderPbr);
    Model* player2 = new Model("res\\content\\models\\random.fbx", &shaderPbr);

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,
                                  (float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    cc1->start();
    cc2->start();
    
    renderer.init();


 /*   sm.getLoadedScenes()[0]->getSceneEntities()[0]->addComponent(cc1);
    sm.getLoadedScenes()[0]->getSceneEntities()[1]->addComponent(cc2);

    sm.getLoadedScenes()[0]->getSceneEntities()[0]->addComponent(sphere);
    sm.getLoadedScenes()[0]->getSceneEntities()[1]->addComponent(sphere);
*/
	sm.getLoadedScenes()[0]->addEntity(new Entity("player"));
	sm.getLoadedScenes()[0]->getSceneEntities()[2]->addComponent(player);
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    sm.getLoadedScenes()[0]->addEntity(new Entity("club"));
    sm.getLoadedScenes()[0]->getSceneEntities()[3]->addComponent(club);
    club->getTransform()->setPosition(glm::vec3(0, -5, 0));
    club->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    sm.getLoadedScenes()[0]->addEntity(new Entity("sphere"));
    sm.getLoadedScenes()[0]->getSceneEntities()[4]->addComponent(sphere);
    sphere->getTransform()->setPosition(glm::vec3(-5.0f, 7.0f, 0.0f));

    sm.getLoadedScenes()[0]->addEntity(new Entity("player2"));
    sm.getLoadedScenes()[0]->getSceneEntities()[5]->addComponent(player2);
    player->getTransform()->setPosition(glm::vec3(-7, -2, 1));
    while (!glfwWindowShouldClose(s.window))
	{
        //EditorLayer::Gizmos::Clear();

		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = s.camera.GetViewMatrix();

        std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
		//glm::mat4 view = playerCamera->getView();

        imgui_begin();
		editor.draw();
        
		
		renderer.updateProjectionAndView(projection, view);
        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
        sm.updateLoadedScenes();
        //scene.update();
        cm.update();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //arcadeRenderer->renderText();


        //TODO: Kuba: Muszę poprawić renderowanie textu u siebie
        //hud
        //counterRenderer->renderAndUpdateCounter(shaderText, s.deltaTime, 300, 160, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);//if rectangle display queue broken, uncomment glDisabe/glEnable
        //glDisable(GL_DEPTH_TEST);
        //glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(s.WINDOW_WIDTH), 0.0f, static_cast<float>(s.WINDOW_HEIGHT));
        //shader.setMat4("projection", orthoProjection);

        //progressBar.update(s.deltaTime);
        //progressBar.renderBar();

        //image.render();
       // backgroundImage.render();
        //backgroundImage.update(s.deltaTime);
        //glEnable(GL_DEPTH_TEST);
        //hud end


        sm.updateLoadedScenes();

        cm.update();
        //arcadeRenderer->update();
        update();
	}
    a.end();

	end();
	return 0;
}
