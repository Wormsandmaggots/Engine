#include "windows.h"
#include "Debug/Profiler.h"
#include "tracy/TracyOpenGL.hpp"
#include "Text/Text.h"
#include "Engine/Engine.h"
#include "Editor/Editor.h"
#include "Core/AssetManager/AssetManager.h"
#include "ThirdPersonCamera.h"
#include "RigPrep.h"
#include "Animation/InverseKinematics.h"
#include "Physics/ColliderComponent.h"
#include "Physics/CollisionManager.h"
#include "Editor/Gizmos.h"
#include "Input/DebugInput.h"
#include "Input/PlayerInput.h"
#include "HUD/ProgressBar.h"
#include "HUD/BackgroundImage.h"
#include "Renderer/MaterialAsset.h"
#include "Animation/Animation.h"
#include "Animation/Animator.h"
#include "Animation/LookAt.h"
#include "Core/Utils/MathUtils.h"
#include "Core/Utils/Ease.h"
#include "Renderer/InstancedRobots.h"

using namespace SceneManagement;
using namespace utils;

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

    DebugInput debugInput;

    PlayerInput playerInput(GLFW_JOYSTICK_1);
    PlayerInput playerInput1(GLFW_JOYSTICK_2);
    //HID - test

#pragma endregion TEST

    Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");
    Shader collisionTestShader("res/content/shaders/vertex.glsl", "res/content/shaders/collisionTest.frag");
    Shader shaderText("res/content/shaders/vertexText.glsl", "res/content/shaders/fragmentText.glsl");
    Shader colorShader("res/content/shaders/color_v.glsl", "res/content/shaders/color_f.glsl");
    Shader shaderPbr("res/content/shaders/vertexPbr.glsl", "res/content/shaders/fragmentPbr.glsl");
    //Shader shaderCel("res/content/shaders/vertex.glsl", "res/content/shaders/fragmentCel.glsl");
    Shader shaderRig("res/content/shaders/vertexRig.glsl", "res/content/shaders/fragment.glsl");
    Shader shaderRigInstanced("res/content/shaders/vertexRigInstanced.glsl", "res/content/shaders/fragment.glsl");

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
    renderer.addShader(&shaderRig);

    Model* player = new Model("res/content/models/NEWCHARACTER/Character_fixed_kurwa_origin.fbx", &shaderRig);
    //Model* player = new Model("res/content/models/npc1/pykpykKurwaMaDzialac.fbx", &shaderRig);
    RigPrep* playerRig = new RigPrep(player);
    InverseKinematics* playerIK = new InverseKinematics(playerRig);
    LookAt* playerLA = new LookAt(playerRig);

    int offset = 0;
    glm::vec2 joystickOffset = glm::vec2(0,0);
    glm::vec2 joystickOffset2 = glm::vec2(0,0);
    renderer.addShader(&shaderText);
    renderer.addShader(&shaderPbr);
    renderer.addShader(&shaderRigInstanced);
    //renderer.addShader(&shaderCel);

    //Model* club = new Model("res/content/models/club2/club2.obj", &shaderPbr);
    Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &collisionTestShader);
    Model* drink = new Model("res/content/models/kieliszki/drineczki.fbx");
    Model* canisterModel = new Model("res/content/models/Canister/Canister/kanistry.fbx");
    Model* chairsModel = new Model("res/content/models/krzesla/krzeslo/krzesla.fbx");
    Model* player2 = new Model("res/content/models/barman/barman_animated.fbx", &shaderRig);
    InstancedRobots* ir = new InstancedRobots("res/content/models/barman/barman_animated.fbx",
                                              glm::vec2(5, 5),
                                              &shaderRigInstanced,
                                              glm::vec3(0),
                                              glm::vec3(70,0,70),
                                              glm::vec3(0.01f));
    Animation* npcAnimation = new Animation("res/content/models/barman/barman_animated.fbx", ir);
    RigPrep* npcRig = new RigPrep(ir);
    Animator* animator = new Animator(npcAnimation);
    //LookAt* npcLA = new LookAt(npcRig);
    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("SCORE", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    cc1->start();
    cc2->start();
/*
    Entity* player1 = new Entity("player1");
	sm.getLoadedScenes()[0]->addEntity(player1);
    player1->addComponent(player);
    player1->getTransform()->setPosition(glm::vec3(-2, -2, 1));
    //player->getTransform()->setPosition(glm::vec3(10, 0, 0));
    player1->getTransform()->setScale(glm::vec3(0.01f, 0.01f, 0.01f));


    Entity* handPointer = new Entity("handPointer");
    //sm.getLoadedScenes()[0]->addEntity(handPointer);
    handPointer->setParent(*player1);
    handPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition()* 0.01f);
*/
//    Entity* club1 = new Entity("club");
//    sm.getLoadedScenes()[0]->addEntity(club1);
//    club1->addComponent(club);
//    club->getTransform()->setPosition(glm::vec3(0, -5, 0));
//    club->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Entity* sphere1 = new Entity("sphere");
    sm.getLoadedScenes()[0]->addEntity(sphere1);
    sphere1->addComponent(ir);
    //sphere1->getTransform()->setPosition(glm::vec3(-5.0f, 7.0f, 0.0f));
    sphere1->getTransform()->setScale(glm::vec3(1.f));

    Entity* drink1 = new Entity("drink");
    sm.getLoadedScenes()[0]->addEntity(drink1);
    drink1->addComponent(drink);
    drink->getTransform()->setPosition(glm::vec3(0, -5, 0));
    drink->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Entity* canisters = new Entity("canisters");
    sm.getLoadedScenes()[0]->addEntity(canisters);
    canisters->addComponent(canisterModel);
    canisterModel->getTransform()->setPosition(glm::vec3(0, -5, 0));
    canisterModel->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    //canisterModel->getTransform()->rotate(glm::vec3(90.0f, 0.0f, 0.0f));

    Entity* chairs = new Entity("chairs");
    sm.getLoadedScenes()[0]->addEntity(chairs);
    chairs->addComponent(chairsModel);
    chairsModel->getTransform()->setPosition(glm::vec3(0, -5, 0));
    chairsModel->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);
    //player2->getTransform()->setScale(glm::vec3(0.05f, 0.05f, 0.05f));
    player2->getTransform()->setPosition(glm::vec3(-3.0f, 7.0f, 0.0f));

    bool f = true;
    while (!glfwWindowShouldClose(s.window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);
        playerInput.interpretInput();
        playerInput1.interpretInput();
        offset += debugInput.interpretIKInput(s.window, s.camera, s.deltaTime);
        animator->UpdateAnimation(s.deltaTime);
        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();

        //std::cout << sphere->getTransform()->getLocalPosition().x << " " << sphere->getTransform()->getLocalPosition().y << " "<< sphere->getLocalTransform()->getLocalPosition().z << " "<<std::endl;
        //glm::mat4 projection = playerCamera->getProjection((float)s.WINDOW_WIDTH ,(float)s.WINDOW_HEIGHT);
        //glm::mat4 view = playerCamera->getView();

        imgui_begin();
        editor.draw();


        shaderRig.use();

        ///IK


        joystickOffset = playerInput.getJoystick(1);
        joystickOffset2 = playerInput.getJoystick(2);

        //LOG_INFO(to_string(joystickOffset.y));

        joystickOffset.x = Math::Remap(
                easeInOutQuint(Math::Remap(joystickOffset.x, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset.y = Math::Remap(
                easeInOutQuint(Math::Remap(joystickOffset.y, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset2.x = Math::Remap(
                easeInOutQuint(Math::Remap(joystickOffset2.x, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset2.y = Math::Remap(
                easeInOutQuint(Math::Remap(joystickOffset2.y, -1, 1, 0 ,1)),
                0, 1, -1, 1);

        joystickOffset *= 100 * s.deltaTime;
        joystickOffset2 *= 100 * s.deltaTime;

        playerIK->update(joystickOffset[0], -joystickOffset[1], "mixamorig:RightHand");
        playerIK->update(joystickOffset2[0], -joystickOffset2[1], "mixamorig:LeftHand");
        playerIK->update(offset, offset, "mixamorig:RightHand");
        playerRig->update();
//        auto transforms = playerRig->GetFinalBoneMatrices();
//        for (int i = 0; i < transforms.size(); ++i)
//            shaderRig.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
//        handPointer->getTransform()->setPosition(playerRig->getBone("mixamorig:RightHand")->getModelPosition() * 0.01f);
//        playerLA->update(90);


        ///ANIMATIONS
        shaderRigInstanced.use();
        npcRig->update();
        auto transforms = animator->GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            shaderRigInstanced.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
        //npcLA->update(90);

        f = false;
        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere1->getTransform()->getLocalPosition());
        renderer.updateProjectionAndView(projection, view);
        sm.updateLoadedScenes();
        //scene.update();
        cm.update();
        arcadeRenderer->renderText();

        update();
    }
    a.end();

    end();
    return 0;
}