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
    EditorLayer::Editor editor;
    editor.init(&s.camera);
    CollisionManager cm;
    AudioManager audioManager;
    audioManager.init();

    Sound* clap = audioManager.loadSound("res/content/sounds/clap.wav");
    Sound* sweep = audioManager.loadSound("res/content/sounds/sweep.wav");
    clap->setVolume(0.2);
    sweep->setVolume(0.4);

    SceneManager sm;


    float songSampleInterval = 0.3;
    vector<SongSample> songData;
    const char* path = "res/content/sounds/queen.wav";
    Sound* song = audioManager.loadSound(path);


    SongAnalizer::parseSong(songSampleInterval, path, songData);
    SongAnalizer::testparseSong(songSampleInterval, path, songData);


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
    // Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    Shader shaderRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/fragment.glsl");
    //TODO: Kuba: Czy to może tutaj zostać?

  


    Renderer renderer(&shader);
    renderer.init();

    Model* club = new Model("res/content/models/club2/club2.obj", &shaderPbr);
    //Model* player = new Model("res\\content\\models\\player\\character_base.obj", &shaderPbr);
    //Model* player2 = new Model("res/content/models/random.fbx", &shaderPbr);

    Model* playerModel = new Model("res/content/models/Chlop/Main_character.fbx", &shaderRig);
    RigPrep* playerRig = new RigPrep(playerModel);
    InverseKinematics* playerIK = new InverseKinematics(playerRig);

    glm::vec2 joystickOffset = glm::vec2(0);
    glm::vec2 joystickOffset2 = glm::vec2(0);
    glm::vec2 joystickOffset3 = glm::vec2(0);
    glm::vec2 joystickOffset4 = glm::vec2(0);

    Text* comboRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* scoreRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");


    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();


    float time = 0;

    Spawner spawner(sm.getLoadedScenes().at(0));
    float timeToDispense = songSampleInterval;
    float timeToDispense2 = timeToDispense;


    Model* sphereModel = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color.json"));
    Model* sphereModel_green = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"));
    Model* sphereModel_green2 = new Model("res/content/models/sphere/untitled.obj", new MaterialAsset("res/content/materials/color_green.json"));

    Entity* lowBall = new Entity("lowBall");
    lowBall->getTransform()->setScale(glm::vec3(.3));
    lowBall->getTransform()->setPosition(glm::vec3(0));
    lowBall->addComponent(sphereModel_green);
    //currentScene->addEntity(lowBall);

    Entity* highBall = new Entity("highBall");
    highBall->getTransform()->setScale(glm::vec3(.3));
    highBall->getTransform()->setPosition(glm::vec3(1, 1, 0));
    highBall->addComponent(sphereModel_green2);
    //currentScene->addEntity(highBall);


    Entity* player = new Entity("Player");
    //player->getTransform()->setRotation(glm::vec3(0, 180, 0));
    player->addComponent(playerModel);
    player->getTransform()->setPosition(glm::vec3(0, -2.5, 0));
    player->getTransform()->setScale(glm::vec3(0.01f));
    currentScene->addEntity(player);


    Entity* leftHandPointer = new Entity("leftHandPointer");
    ColliderComponent* lHandcollider = new ColliderComponent();
    lHandcollider->start();
    lHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
    leftHandPointer->setParent(*player);
    leftHandPointer->addComponent(lHandcollider);
    leftHandPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);

    Entity* rightHandPointer = new Entity("rightHandPointer");
    ColliderComponent* rHandcollider = new ColliderComponent();
    rHandcollider->start();
    rHandcollider->getCollider()->getColliderShape()->setRadius(0.08);
    rightHandPointer->setParent(*player);
    rightHandPointer->addComponent(rHandcollider);
    rightHandPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);

    Entity* leftFootPointer = new Entity("leftFootPointer");
    ColliderComponent* leftFootCollider = new ColliderComponent();
    leftFootCollider->start();
    leftFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
    leftFootPointer->setParent(*player);
    leftFootPointer->addComponent(leftFootCollider);
    leftFootPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);

    Entity* rightFootPointer = new Entity("rightFootPointer");
    ColliderComponent* rightFootCollider = new ColliderComponent();
    rightFootCollider->start();
    rightFootCollider->getCollider()->getColliderShape()->setRadius(0.08);
    rightFootPointer->setParent(*player);
    rightFootPointer->addComponent(rightFootCollider);
    rightFootPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);





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


        comboRenderer->setParameters("Score " + std::to_string(score), 100, 100, 1.0f, glm::vec3(0.6, 0.9f, 0.3f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);
        scoreRenderer->setParameters("Combo " + std::to_string(combo) + "x", 100, 150, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float)s.WINDOW_WIDTH, (float)s.WINDOW_HEIGHT);


        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();


        timeToDispense2 -= s.deltaTime;
        if (start) {
            if (timeToDispense2 < 0 && songDataIndex < songData.size()) {
                switch (songData[songDataIndex].type) {
                case sampleType::CLAP:
                    //spawner.spawnBall("ball", glm::vec3(0, 1, 0), clap, sweep);
                    //spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x * 1.25, 1 * 1.25, 0), clap, sweep);
                    spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].high.x, 1, 0), clap, sweep);

                    break;
                case sampleType::BASS:
                    //spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x * 1.25, -1 * 1.25, 0), clap, sweep);
                    spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x, -1, 0), clap, sweep);

                    //spawner.spawnBall("ball", glm::vec3(0, -1, 0), clap, sweep);
                    break;
                case sampleType::SKIP:
                    break;
                }
                songDataIndex++;
                timeToDispense2 = timeToDispense;
                if (!(songDataIndex < songData.size())) songDataIndex = 0;
            }
            song->play();




            //if (timeToDispense2 < 0 && songDataIndex < songData.size()) 
            //{
            // if(timeToDispense2 < 0 && songDataIndex < songData.size())
            //    spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x*1.25, songData[songDataIndex].high.x * 1.25, -7), clap, sweep);

            //        if (songData[songDataIndex].bass.x > 0.5) 
            //        {
            //           // spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x*1.25, songData[songDataIndex].high.x * 1.25, -7), clap, sweep);

            //        }
            //        else
            //        {
            //            //spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].high.x * 1.25, songData[songDataIndex].mid.x * 1.25, -7), clap, sweep);

            //        }

            //        if (songData[songDataIndex].bass.x < 0.5) 
            //        {
            //            //spawner.spawnBall("ball", glm::vec3(songData[songDataIndex].mid.x * 1.25, songData[songDataIndex].high.x * 1.25, -7), clap, sweep);

            //        }
            //        else 
            //        {
            //            //spawner.spawnBall("ball", glm::vec3(songData[songDataIndex].high.x * 1.25, songData[songDataIndex].mid.x * 1.25, -7), clap, sweep);
            //        }

            //    //spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].bass.x, songData[songDataIndex].bass.y, -2.5), clap, sweep);
            //    //spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].mid.x, songData[songDataIndex].mid.y, -5* songData[songDataIndex].bass.x), clap, sweep);
            //    //spawner.spawnBall("ball", glm::vec3(-songData[songDataIndex].high.x, songData[songDataIndex].high.y, -5* songData[songDataIndex].bass.x), clap, sweep);

            //    //spawner.spawnDrink("drink",glm::vec3(1.25,0,-5),clap,sweep);

            //    songDataIndex++;
            //    timeToDispense2 = timeToDispense;
            //}
            //else {
            //    songDataIndex++;
            //    timeToDispense2 = timeToDispense;

            //}
            //if (!(songDataIndex < songData.size())) songDataIndex = 0;

        }




        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getTransform()->getLocalPosition().z << " "<<std::endl;
        //glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
        //glm::mat4 view = playerCamera->getView();

        imgui_begin();
        editor.draw();

        shaderRig.use();

        joystickOffset2 = playerInput.getJoystick(2) * 100.0f;
        playerIK->update(-joystickOffset2[0], -joystickOffset2[1], "mixamorig:RightHand");
        playerRig->update();


        joystickOffset = playerInput.getJoystick(1) * 100.0f;
        playerIK->update(-joystickOffset[0], -joystickOffset[1], "mixamorig:LeftHand");
        playerRig->update();

        joystickOffset3 = playerInput1.getJoystick(1) * 100.0f;
        playerIK->update(-joystickOffset3[0], -joystickOffset3[1], "mixamorig:LeftFoot");
        playerRig->update();

        joystickOffset4 = playerInput1.getJoystick(2) * 100.0f;
        playerIK->update(-joystickOffset4[0], -joystickOffset4[1], "mixamorig:RightFoot");
        playerRig->update();

        auto transforms = playerRig->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shaderRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        rightHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:LeftHand")->getModelPosition() * 0.01f);
        leftHandPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);
        rightFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:RightFoot")->getModelPosition() * 0.01f);
        leftFootPointer->getTransform()->setPosition(glm::vec3(0, 0, 0.6) + playerRig->getBone("mixamorig:LeftFoot")->getModelPosition() * 0.01f);


        //shaderPbr.use();
        //shaderPbr.setVec3("camPos",s.camera.Position);
        //shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
        renderer.updateProjectionAndView(projection, view);
        sm.updateLoadedScenes();
        //scene.update();
        cm.update();
        //		ImGui::Render();
        //		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        comboRenderer->renderText();
        scoreRenderer->renderText();




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