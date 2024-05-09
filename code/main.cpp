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
#include "Renderer/FrameBuffer.h"
#include "Frustum.h"
#include "AABBComponent.h"
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
    //CPM_GLM_AABB_NS::AABB* aabb = new CPM_GLM_AABB_NS::AABB();

	sm.loadScene("res/content/maps/test.yaml");

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
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    Shader screenShader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");
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

    //frustum
    // Ustawiamy punkty graniczne dla naszego modelu
// Zakładamy, że funkcja `getModelBounds` zwraca minimalne i maksymalne punkty modelu
    std::pair<glm::vec3, glm::vec3> bounds = sphere->getModelBounds();

// Tworzymy nowy komponent AABB
    AABBComponent* aabbComponent = new AABBComponent();

// Ustawiamy granice dla naszego AABB
    aabbComponent->extend(bounds.first);
    aabbComponent->extend(bounds.second);

// Dodajemy komponent AABB do naszego modelu
    sphere1->addComponent(aabbComponent);

    // Ustawiamy punkty graniczne dla naszego modelu
// Zakładamy, że funkcja `getModelBounds` zwraca minimalne i maksymalne punkty modelu
    std::pair<glm::vec3, glm::vec3> playerBounds = player2->getModelBounds();

// Tworzymy nowy komponent AABB
    AABBComponent* playerAabbComponent = new AABBComponent();

// Ustawiamy granice dla naszego AABB
    playerAabbComponent->extend(playerBounds.first);
    playerAabbComponent->extend(playerBounds.second);

// Dodajemy komponent AABB do naszego modelu
    player3->addComponent(playerAabbComponent);

    // Parametry frustum
    float aspect = s.WINDOW_WIDTH / s.WINDOW_HEIGHT; // Zakładamy, że s.WINDOW_WIDTH i s.WINDOW_HEIGHT to szerokość i wysokość okna
    float fovY = glm::radians(45.0f); // Pionowe pole widzenia w radianach
    float zNear = 0.1f; // Bliska płaszczyzna odcięcia
    float zFar = 100.0f; // Daleka płaszczyzna odcięcia

// Tworzenie frustum
    Frustum frustum = createFrustumFromCamera(s.camera, aspect, fovY, zNear, zFar);

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    FrameBuffer* fb = new FrameBuffer(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    while (!glfwWindowShouldClose(s.window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        fb->bind();

        glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = s.camera.GetViewMatrix();

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
		//glm::mat4 view = playerCamera->getView();

        imgui_begin();
        editor.draw();

        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
		renderer.updateProjectionAndView(projection, view);
        sm.updateLoadedScenes();
        //scene.update();
        cm.update();
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        arcadeRenderer->renderText();

        fb->unbind();

        screenShader.use();
        fb->drawQuad();


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


//        sm.updateLoadedScenes();
//
//        cm.update();
        //arcadeRenderer->update();
        update();
	}
    a.end();

	end();
	return 0;
}
