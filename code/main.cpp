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
#include "Renderer/FrameBuffer.h"
#include "Renderer/SSAO.h"
#include "HUD/Image.h"
#include "HUD/ResizableImage.h"
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
    Shader imageShader("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d.glsl");


    MaterialAsset material("res/content/materials/color.json");

    SSAO ssao;
    ssao.create(s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

    Renderer renderer(&ssao.shaderGeometryPass);
    renderer.init();

    renderer.addShader(&collisionTestShader);
    renderer.addShader(&colorShader);
    renderer.addShader(material.getShader());//TODO: Automatyczne dodawanie shadera do updatowania MVP

    Model* player = new Model("res/content/models/player/character_base.obj");

    renderer.addShader(&shaderText);
    renderer.addShader(&shaderPbr);
    renderer.addShader(&shaderCel);
    renderer.addShader(&ssao.shaderGeometryPass);
    renderer.addShader(&imageShader);


    Model* club = new Model("res/content/models/club2/club2.obj", &ssao.shaderGeometryPass);
    Model* sphere = new Model("res\\content\\models\\sphere\\untitled.obj", &ssao.shaderGeometryPass);
    Model* player2 = new Model("res/content/models/random.fbx", &ssao.shaderGeometryPass);

    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();

    ColliderComponent* cc1 = new ColliderComponent();
    ColliderComponent* cc2 = new ColliderComponent();

    cc1->start();
    cc2->start();

    //menu
    Image* menu = new Image(&imageShader);
    ResizableImage* bar = new ResizableImage(&imageShader);


    Entity* player1 = new Entity("player");
    sm.getLoadedScenes()[0]->addEntity(player1);
    player1->addComponent(player);
    player->getTransform()->setPosition(glm::vec3(-5, -2, 1));

    Entity* club1 = new Entity("club");
    sm.getLoadedScenes()[0]->addEntity(club1);
    club1->addComponent(club);
    club->getTransform()->setPosition(glm::vec3(0, -5, 0));
    club->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));

    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);

    Entity* sphere1 = new Entity("sphere");
    sm.getLoadedScenes()[0]->addEntity(sphere1);
    sphere1->addComponent(sphere);
    sphere->getTransform()->setPosition(lightPos);

    Entity* player3 = new Entity("player2");
    sm.getLoadedScenes()[0]->addEntity(player3);
    player3->addComponent(player2);
    player->getTransform()->setPosition(glm::vec3(-7, -2, 1));

    //menu
    Entity* mainMenu = new Entity("menu");
    sm.getLoadedScenes()[0]->addEntity(mainMenu);
    mainMenu->addComponent(menu);
    menu->getTransform()->setScale(glm::vec3(0.25f, 0.25f, 0.25f));
    menu->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    Entity* mainBar = new Entity("bar");
    sm.getLoadedScenes()[0]->addEntity(mainBar);
    mainBar->addComponent(bar);
    bar->getTransform()->setScale(glm::vec3(0.1f, 0.3f, 0.0f));
    bar->getTransform()->setPosition(glm::vec3(-0.5f, 0.0f, 0.0f));
    //TODO: Rotacja nie działa
    //bar->getTransform()->setRotation(glm::vec3(45.0f, 45.0f, 0.0f));

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

    static float linear    = 0.09f;
    static float quadratic = 0.032f;
    static float power = 1;
    static float kernelSize = 64;
    static float radius = 0.5f;
    static float bias = 0.025f;
    static bool onlySSAO = true;
    static vec2 range(2,2);
    static float mul = 4;
    static float texelSize = 1;

    //for resizing bar
    double lastTime = 0.0;

    while (!glfwWindowShouldClose(s.window))
    {
        imgui_begin();
        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;
        debugInput.interpretInput(s.window, s.camera, s.deltaTime);

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom),(float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = s.camera.GetViewMatrix();



        ImGui::Begin("SSAO");

        {
            ImGui::DragFloat3("light Color", glm::value_ptr(lightColor));
            ImGui::DragFloat("linear", &linear);
            ImGui::DragFloat("quadratic", &quadratic);
            ImGui::DragFloat("power", &power);
            ImGui::DragFloat("kernelSize", &kernelSize);
            ImGui::DragFloat("radius", &radius);
            ImGui::DragFloat("bias", &bias);
            ImGui::DragFloat2("range", glm::value_ptr(range));
            ImGui::DragFloat("multiplier", &mul);
            ImGui::DragFloat("texel size", &texelSize);
            ImGui::Checkbox("Only SSAO", &onlySSAO);
        }

        ImGui::End();

        shaderPbr.use();
        shaderPbr.setVec3("camPos",s.camera.Position);
        shaderPbr.setVec3("lightPos",sphere->getTransform()->getLocalPosition());
        ssao.shaderGeometryPass.use();
        ssao.shaderGeometryPass.setBool("invertedNormals", false);
        ssao.shaderGeometryPass.setBool("onlySSAO", onlySSAO);
        renderer.updateProjectionAndView(projection, view);
        glBindFramebuffer(GL_FRAMEBUFFER, ssao.gBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        editor.draw();

        sm.updateLoadedScenes();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, ssao.ssaoFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssao.shaderSSAO.use();
        ssao.shaderSSAO.setFloat("power", power);
        ssao.shaderSSAO.setFloat("kernelSize", kernelSize);
        ssao.shaderSSAO.setFloat("radius", radius);
        ssao.shaderSSAO.setFloat("bias", bias);
        // Send kernel + rotation
        for (unsigned int i = 0; i < 64; ++i)
        {
            if(i > kernelSize)
            {
                ssao.shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", vec3(0));
            }
            else
                ssao.shaderSSAO.setVec3("samples[" + std::to_string(i) + "]", ssao.ssaoKernel[i]);
        }

        ssao.shaderSSAO.setMat4("projection", projection);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ssao.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.noiseTexture);
        ssao.renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, ssao.ssaoBlurFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        ssao.shaderSSAOBlur.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBuffer);
        ssao.shaderSSAOBlur.setInt("rangeX", range.x);
        ssao.shaderSSAOBlur.setInt("rangeY", range.y);
        ssao.shaderSSAOBlur.setFloat("mul", mul);
        ssao.shaderSSAOBlur.setFloat("texelSize", texelSize);
        ssao.renderQuad();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ssao.shaderLightingPass.use();
        glm::vec3 lightPosView = glm::vec3(s.camera.GetViewMatrix() * glm::vec4(sphere1->getTransform()->getLocalPosition(), 1.0));
        ssao.shaderLightingPass.setVec3("light.Position", lightPosView);
        ssao.shaderLightingPass.setVec3("light.Color", lightColor);


        ssao.shaderLightingPass.setFloat("light.Linear", linear);
        ssao.shaderLightingPass.setFloat("light.Quadratic", quadratic);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ssao.gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, ssao.gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, ssao.gAlbedo);
        glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
        glBindTexture(GL_TEXTURE_2D, ssao.ssaoColorBufferBlur);
        ssao.renderQuad();


        //HUD Menu
        glDisable(GL_DEPTH_TEST);
        imageShader.use();
        //imageShader.setMat4("view", view);
        //imageShader.setMat4("projection", projection);
        menu->renderPlane();
        bar->renderPlane();
        glEnable(GL_DEPTH_TEST);

        double currentTime = glfwGetTime();
        if (currentTime - lastTime >= 3.0)
        {
            bar->resizeOnImpulse(0.1f);
            lastTime = currentTime;
        }

        cm.update();

        update();
    }
    a.end();

    end();
    return 0;
}