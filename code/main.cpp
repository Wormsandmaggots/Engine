#include "windows.h"
#include <iostream>
#include "Engine/Engine.h"

#include "tracy/TracyOpenGL.hpp"
#include "Editor/Editor.h"
#include "Input/WindowsInput.h"

using namespace SceneManagement;


//#define PROFILER

int main()
{
	
#if defined(PROFILER)
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Tworzenie struktury PROCESS_INFORMATION
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    CreateProcess(NULL, (LPSTR)"res/tracyExe/Tracy.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
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
	editor.setCamera(&s.camera);

	AudioManager a;
	a.init();

	Sound* sound = a.loadSound("res/content/sounds/Ich will.mp3", "testSound");
	SceneManager sm;

	sm.loadScene("res/content/maps/exampleScene.yaml");
	sound->play();

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
    Input::s_Instance = new WindowsInput(s.window);

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

	Scene2 scene("scene");
	Entity* entity = new Entity("nanosuit");
	Entity* monke = new Entity("monke");
	// Entity* airplane = new Entity("airplane");

	Model* model = new Model("res\\content\\models\\nanosuit\\nanosuit.obj");
	Model* monkeModel = new Model("res\\content\\models\\plane.obj");
	// Model* airplaneModel = new Model("res\\content\\models\\aircraft\\airplane.obj");

	entity->addComponent(model);
	monke->addComponent(monkeModel);
	// airplane->addComponent(airplaneModel);

	scene.addEntity(entity);
	scene.addEntity(monke);
	// scene.addEntity(airplane);

	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
	Renderer renderer(shader, scene.getSceneEntities());

	monke->getTransform()->setPosition(glm::vec3(5, 3, 1));
	// airplane->getTransform()->setPosition(glm::vec3(-5, 0, 1));

	while (!glfwWindowShouldClose(s.window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;



		processInput(s.window);

		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),
		                                        (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = s.camera.GetViewMatrix();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		imgui_begin();
		editor.drawHierarchy();

		//        Gizmos::editTransform(glm::value_ptr(view),
		//                              glm::value_ptr(projection),
		//                              glm::value_ptr(monke->getTransform()->getWorldMatrix()));

		FrameMark;
		renderer.renderModels();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(s.window);
		glfwMakeContextCurrent(s.window);
		glfwPollEvents();
	}

#if defined(PROFILER)
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
#endif

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(s.window);
	glfwTerminate();


	return 0;
}
