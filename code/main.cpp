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
#include "Generative-System/SongAnalizer.h"
#include "Generative-System/Ball.h"

#include "Globals.h"
#include "Input/DebugInput.h"
#include "Input/PlayerInput.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "RigPrep.h"
#include "Animation/InverseKinematics.h"
#include "ECS/ScriptableEntity.h"

using namespace SceneManagement;

int main() {
    init();
	EditorLayer ::Editor editor;
    editor.init(&s.camera);
    CollisionManager cm;
	AudioManager audioManager;
    audioManager.init();
	Sound* sound = audioManager.loadSound("res/content/sounds/overcompensate.wav");
	SceneManager sm;
    

    float songSampleInterval = 1;
    vector<SongSample> songData;
    SongAnalizer::parseSong(songSampleInterval, "res/content/sounds/overcompensate.wav",songData);
    int songDataIndex = 0;

    DebugInput debugInput;

    PlayerInput playerInput(GLFW_JOYSTICK_1);
    PlayerInput playerInput1(GLFW_JOYSTICK_2);
   
	sm.loadScene("res/content/maps/Marcin.yaml");

    Scene2* currentScene = sm.getLoadedScenes().at(0);


    //HID - test
    //TODO: Kuba: Czy to może tutaj zostać?
    Input::getInstance().initializeController(GLFW_JOYSTICK_1);
    //HID - test
    //HID - test


	Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
	Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
	Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    Shader shaderRig("res/content/shaders/vertexRig.glsl","res/content/shaders/fragment.glsl");
    //TODO: Kuba: Czy to może tutaj zostać?

    //HUD
    ProgressBar progressBar("res/content/shaders/vertex_2d.glsl", "res/content/shaders/progress_bar_fragment.glsl", "res/content/textures/bar.png", 100.0f);
    BackgroundImage backgroundImage("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/nodes.png");
    Image image("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl", "res/content/textures/hud_back.png");
    
  

    Renderer renderer(&shader);
    renderer.init();

    Model* club = new Model("res/content/models/club2/club2.obj", &shaderPbr);
	//Model* player = new Model("res\\content\\models\\player\\character_base.obj", &shaderPbr);
    //Model* player2 = new Model("res/content/models/random.fbx", &shaderPbr);
    
    Model* playerModel = new Model("res/content/models/Character_rigged/originWstopkach.fbx", &shaderRig);
    RigPrep* playerRig = new RigPrep(playerModel);
	InverseKinematics* playerIK = new InverseKinematics(playerRig);

    glm::vec2 joystickOffset = glm::vec2(0);
    

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("score", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    //ColliderComponent* cc1 = new ColliderComponent();
    //ColliderComponent* cc2 = new ColliderComponent();

    //cc1->start();
    //cc2->start();


    float time = 0;

    Spawner spawner(sm.getLoadedScenes().at(0));
    float timeToDispense = songSampleInterval;
    float timeToDispense2 = timeToDispense;


    Model* sphereModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
    Model* sphereModel_green = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"));

  

    Entity* player = new Entity("Player");
    player->addComponent(playerModel);
    player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
    player->getTransform()->setScale(glm::vec3(0.01f));
    sm.getLoadedScenes().at(0)->addEntity(player);


    Entity* handPointer = new Entity("handPointer");
    ColliderComponent* collider = new ColliderComponent();
    collider->start();
    collider->getCollider()->getColliderShape()->setRadius(0.12);
    handPointer->setParent(*player);
    handPointer->addComponent(collider);
	handPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition()*0.01f);


    
    sound->play();
    sound->setVolume(1.f);
    


    while (!glfwWindowShouldClose(s.window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		s.deltaTime = currentFrame - s.lastFrame;
		s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        time = time + s.deltaTime;
        deltaTime = s.deltaTime;

		debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();
        
        glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = s.camera.GetViewMatrix();

        timeToDispense2 -= s.deltaTime;



        if ( timeToDispense2<0 && songDataIndex < songData.size())
        {
            spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].bass.x, -songData[songDataIndex].bass.y, -20));
            spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x, songData[songDataIndex].mid.y, -20));
            spawner.spawnBall("ball", glm::vec3(songData[songDataIndex].high.x, songData[songDataIndex].high.y, -20));
            //spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, -songData[songDataIndex].high.y, -20), sphereModel);
            
            songDataIndex++;
			timeToDispense2 = timeToDispense;
        }
            

       
       //spawner.updateBalls();
       //spawner.removeBalls(s.deltaTime);

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
		//glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
		//glm::mat4 view = playerCamera->getView();

        imgui_begin();
        editor.draw();

        shaderRig.use();
        joystickOffset = playerInput.getJoystick(1) * 100.0f;
        playerIK->update(joystickOffset[0], -joystickOffset[1], "mixamorig:RightHand");
        //playerIK->update(offset, offset, "mixamorig:RightHand");
        playerRig->update();
        auto transforms = playerRig->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shaderRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        handPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.007f);

        //shaderPbr.use();
        //shaderPbr.setVec3("camPos",s.camera.Position);
        //shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
		renderer.updateProjectionAndView(projection, view);
        sm.updateLoadedScenes();
        //scene.update();
        cm.update();
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        arcadeRenderer->renderText();
        



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
    audioManager.end();

	end();
	return 0;
}
