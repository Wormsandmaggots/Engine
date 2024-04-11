#include "windows.h"
#include <iostream>
#include "Debug/Profiler.h"
#include "tracy/TracyOpenGL.hpp"
#include "Text/Text.h"

//#define PROFILER
#if defined(PROFILER) //overloading operators new and delete globally for profiling
void* operator new(std::size_t count)
{
    auto ptr = malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}
void operator delete(void* ptr) noexcept
{
    TracyFree(ptr);
    free(ptr);
}
#endif

#include "Engine/Engine.h"
#include "Editor/Editor.h"
#include "Core/AssetManager/AssetManager.h"
#include "ThirdPersonCamera.h"

using namespace SceneManagement;

int main() {
    #if defined(PROFILER)
        Profiler::get().init();
    #endif

	if (GLFWInit())
	{
		LOG_ERROR("Failed to initialize GLFW");
		return -1;
	}

	LOG_INFO("GLFW initialized");

	//Radek note: don't mind me, just testing
#pragma region TEST
	EditorLayer::Editor editor;
    editor.init(&s.camera);

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

	glm::vec3 v = {0, 0, 0};
	glm::vec3 v2 = {20, 20, 20};
	Collider c1 = Collider(v, 10);
	Collider c2 = Collider(v, 5);
	Collider c3 = Collider(v2, 5);


	if (c1.CheckCollision(c2))
	{
		LOG_INFO("THAT COLLISION WORKS");
	}

	if (!c1.CheckCollision(c3))
	{
		LOG_INFO("AND THAT DIDNT COLLIDE, as expected");
	}


#pragma endregion TEST

	s.window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);

	if (s.window == nullptr)
	{
		LOG_ERROR("Failed to Create GLFW window");
		glfwTerminate();
		return -1;
	}

	LOG_INFO("GLFW window created");
	glfwMakeContextCurrent(s.window);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");

		glfwDestroyWindow(s.window);
		glfwTerminate();

		return -1;
	}

	LOG_INFO("GLAD initialized");

	//    stbi_set_flip_vertically_on_load(true);

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	SetCallbacks(s.window);

	init_imgui();

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,
                                  (float) s.WINDOW_HEIGHT);
    //ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

	Scene2 scene("scene");
	Entity* entity = new Entity("nanosuit");
	Entity* monke = new Entity("monke");
    Entity* player = new Entity("player");
	// Entity* airplane = new Entity("airplane");

	Model* model = new Model("res\\content\\models\\nanosuit\\nanosuit.obj");
	Model* monkeModel = new Model("res\\content\\models\\plane.obj");
    Model* playerModel = new Model("res\\content\\models\\player\\character_base.obj");
	// Model* airplaneModel = new Model("res\\content\\models\\aircraft\\airplane.obj");

	entity->addComponent(model);
	monke->addComponent(monkeModel);
    player ->addComponent(playerModel);
    //player->addComponent(playerCamera);
	// airplane->addComponent(airplaneModel);
	scene.addEntity(entity);
	scene.addEntity(monke);
    scene.addEntity(player);
	// scene.addEntity(airplane);

	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    //Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
	Renderer renderer(shader, scene.getSceneEntities());
    float yrotation = 0;
	monke->getTransform()->setPosition(glm::vec3(5, 3, 1));
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

	// airplane->getTransform()->setPosition(glm::vec3(-5, 0, 1));

	while (!glfwWindowShouldClose(s.window))
	{
        player->getTransform()->setRotation(glm::vec3(0, yrotation, 0));
        if (glfwGetKey(s.window, GLFW_KEY_S) == GLFW_PRESS)
        {
            yrotation+=1;
        }
        if (glfwGetKey(s.window, GLFW_KEY_A) == GLFW_PRESS)
        {
            yrotation-=1;
        }
		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;
        scene.update();
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

		imgui_begin();
		editor.draw();

		//        Gizmos::editTransform(glm::value_ptr(view),
		//                              glm::value_ptr(projection),
		//                              glm::value_ptr(monke->getTransform()->getWorldMatrix()));

		Profiler::get().markFrame();
        Profiler::get().zoneScope();
		renderer.renderModels();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //arcadeRenderer->renderText();
        arcadeRenderer->update();
		glfwSwapBuffers(s.window);
		glfwMakeContextCurrent(s.window);
		glfwPollEvents();
	}

#if defined(PROFILER)
    Profiler::get().end();
#endif

    a.end();
    AssetManager::end();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(s.window);
	glfwTerminate();


	return 0;
}
