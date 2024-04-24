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
#include "Light/Light.h"

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
    //TODO: oddaj do innej klasy
    glm::vec3 lightPos(-10.0f, 0.0f, 30.0f);

    //HID - test
    //Dowywalenia to do innego pliku
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    //HID - test
    DebugInput debugInput;
    //HID - test

	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
	Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
	Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader lightShader("res/content/shaders/vertex_light.glsl", "res/content/shaders/fragment_light.glsl");
    Shader lampShader("res/content/shaders/lamp_vertex.glsl", "res/content/shaders/lamp_fragment.glsl");

    //HUD
    ProgressBar progressBar("res/content/shaders/vertex_2d.glsl", "res/content/shaders/progress_bar_fragment.glsl", "res/content/textures/bar.png", 100.0f);
    BackgroundImage backgroundImage("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/nodes.png");
    Image image("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/hud_back.png");

    Renderer renderer(&shader);

	renderer.addShader(&collisionTestShader);
	renderer.addShader(&shaderText);

    renderer.addShader(&lightShader);
    renderer.addShader(&lampShader);

	Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &collisionTestShader);
	Model* player = new Model("res\\content\\models\\player\\character_base.obj",&lightShader);

    Model* lamp = new Model("res\\content\\models\\lamp\\lamp.obj", &lampShader);

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

    sm.getLoadedScenes()[0]->getSceneEntities()[0]->addComponent(cc1);
    sm.getLoadedScenes()[0]->getSceneEntities()[1]->addComponent(cc2);

    sm.getLoadedScenes()[0]->getSceneEntities()[0]->addComponent(sphere);
    sm.getLoadedScenes()[0]->getSceneEntities()[1]->addComponent(sphere);

	sm.getLoadedScenes()[0]->addEntity(new Entity("player"));
	sm.getLoadedScenes()[0]->getSceneEntities()[2]->addComponent(player);
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    sm.getLoadedScenes()[0]->addEntity(new Entity("lamp"));
    sm.getLoadedScenes()[0]->getSceneEntities()[3]->addComponent(lamp);
    lamp->getTransform()->setPosition(glm::vec3(-10, 0, 30));


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

		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
		//glm::mat4 view = playerCamera->getView();

        imgui_begin();
		editor.draw();

//light
        //TODO: Kuba: uprzątnij kod świateł
        lightShader.use();
/*      //depraciated
        glm::vec3 objectColor(1.0f, 0.0f, 1.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        lightShader.setVec3("objectColor", objectColor);
        lightShader.setVec3("lightColor", lightColor);
*/
        //light
        lightShader.setVec3("light.position", lightPos); //to zostawiam gdyby lammpa miała się przemieszczać
        lightShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        lightShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        //material
        lightShader.setVec3("material.ambient", 1.0f, 0.0f, 1.0f);
        lightShader.setVec3("material.diffuse", 1.0f, 0.0f, 1.0f);
        lightShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        lightShader.setFloat("material.shininess", 32.0f);

        //Obliczanie znormalizowanegej macierzy modelu player
        glm::mat4 model = sm.getLoadedScenes()[0]->getSceneEntities()[2]->getComponent<Model>()->getModelMatrixInWorldSpace();
        // Obliczanie macierzy normalnej
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        // Przekazanie macierzy normalnej do shadera
        lightShader.setMat3("normalMatrix", normalMatrix);

//end: light

        renderer.updateProjectionAndView(projection, view);
        //obliczanie macierzy normalnej modelu, zrób za każdym razem gdy zmieniam jego położenie

		sm.updateLoadedScenes();
        //scene.update();
        cm.update();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //arcadeRenderer->renderText();

        //hud
        counterRenderer->renderAndUpdateCounter(shaderText, s.deltaTime, 300, 160, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);//if rectangle display queue broken, uncomment glDisabe/glEnable
        //glDisable(GL_DEPTH_TEST);
        glm::mat4 orthoProjection = glm::ortho(0.0f, static_cast<float>(s.WINDOW_WIDTH), 0.0f, static_cast<float>(s.WINDOW_HEIGHT));
        shader.setMat4("projection", orthoProjection);

        progressBar.update(s.deltaTime);
        progressBar.renderBar();

        image.render();
        backgroundImage.render();
        backgroundImage.update(s.deltaTime);
        //glEnable(GL_DEPTH_TEST);
        //hud end


        sm.updateLoadedScenes();

        cm.update();
        arcadeRenderer->update();
        update();
	}
    a.end();

	end();
	return 0;
}
