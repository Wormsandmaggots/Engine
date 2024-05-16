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
#include "AABBComponent.h"
#include "AABB.hpp"
#include "FrustumReady.h"
#include "LineRenderer.h"
#include "Frustum.h"

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
    Frustum frustum;



	sm.loadScene("res/content/maps/test.yaml");

    // Utwórz instancję klasy FrustumReady
    FrustumReady frustumReady;
    LineRenderer lineRenderer;

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
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    //TODO: Kuba: Czy to może tutaj zostać?

    //HUD
    ProgressBar progressBar("res/content/shaders/vertex_2d.glsl", "res/content/shaders/progress_bar_fragment.glsl", "res/content/textures/bar.png", 100.0f);
    BackgroundImage backgroundImage("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/nodes.png");
    Image image("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/hud_back.png");
    
    MaterialAsset material("res/content/materials/color.json");
  

    Renderer renderer(&shader);
    renderer.init();
	renderer.addShader(&collisionTestShader);
    renderer.addShader(&colorShader);
    renderer.addShader(material.getShader());//TODO: Automatyczne dodawanie shadera do updatowania MVP

    Model* player = new Model("res/content/models/player/character_base.obj");
    
	renderer.addShader(&shaderText);
    renderer.addShader(&shaderPbr);
    renderer.addShader(&shaderCel);

    Model* club = new Model("res/content/models/club2/club2.obj", &shaderPbr);
	Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &collisionTestShader);
	//Model* player = new Model("res\\content\\models\\player\\character_base.obj", &shaderPbr);
    Model* player2 = new Model("res/content/models/random.fbx", &shaderPbr);

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    cc1->start();
    cc2->start();

    Entity* player1 = new Entity("player");
	sm.getLoadedScenes()[0]->addEntity(player1);
    player1->addComponent(player);
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    Entity* club1 = new Entity("club");
    sm.getLoadedScenes()[0]->addEntity(club1);
    club1->addComponent(club);
    club->getTransform()->setPosition(glm::vec3(0, -5, 0));
    club->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Entity* sphere1 = new Entity("sphere");
    sm.getLoadedScenes()[0]->addEntity(sphere1);
    sphere1->addComponent(sphere);
    sphere->getTransform()->setPosition(glm::vec3(-5.0f, 7.0f, 0.0f));

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);
    player->getTransform()->setPosition(glm::vec3(-7, -2, 1));

    // Wywołaj metodę addAABBToEntitiesWithModel dla każdej załadowanej sceny
    for (Scene2* scene : sm.getLoadedScenes()) {
        frustumReady.addAABBToEntitiesWithModel(scene);
    }

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
        //glm::mat4 view = s.camera.GetViewMatrix();
        glm::mat4 projection2 = s.camera.GetProjection();
        //frustum.update(projection2 * view);

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
		//glm::mat4 view = playerCamera->getView();

        imgui_begin();
        editor.draw();
        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());

        frustum.update(projection * view);
        int totalAABBObjects = 0;
        int objectsInFrustum = 0;

        for (Scene2* scene : sm.getLoadedScenes()) {
            const std::vector<Entity*>& entities = scene->getSceneEntities();
            for (Entity* entity : entities) {
                // Pobierz komponent AABB obiektu
                AABBComponent* aabbComponent = entity->getComponent<AABBComponent>();
                if (aabbComponent != nullptr) {
                    totalAABBObjects++; // Zwiększ licznik obiektów z AABB

                    // Sprawdź, czy AABB jest wewnątrz Frustum
                    if (frustum.isBoxInFrustum(aabbComponent->getAABB())) {
                        objectsInFrustum++; // Zwiększ licznik obiektów wewnątrz Frustum
                    }
                }
            }
        }
        std::cout << "Liczba obiektów z AABB na scenie: " << totalAABBObjects << " ";
        std::cout << "Liczba obiektów z AABB wewnątrz Frustum: " << objectsInFrustum << std::endl;

		renderer.updateProjectionAndView(projection, view);

        sm.updateLoadedScenes();


        //scene.update();



        cm.update();

//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        arcadeRenderer->renderText();

        update();
	}
    a.end();

	end();
	return 0;
}
