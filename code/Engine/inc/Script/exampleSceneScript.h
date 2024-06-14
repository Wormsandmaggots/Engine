#ifndef ENGINE_EXAMPLESCENESCRIPT_H
#define ENGINE_EXAMPLESCENESCRIPT_H

#include "Script/SceneScript.h"

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
#include "HUD/Image.h"
#include "HUD/Button.h"
#include "HUD/ResizableImage.h"
#include "Renderer/MaterialAsset.h"
#include "Renderer/FrameBuffer.h"
#include "Renderer/SSAO.h"
#include "Generative-System/SongAnalizer.h"
#include "Generative-System/Ball.h"
#include "JoyShockLibrary.h"

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

class exampleSceneScript: public SceneScript{
private:
    SceneManagement::SceneManager& sm;
    glm::vec3& lightColor;
    Shader& shaderPbr;
    Shader* imageShader;
    Shader* imageShaderGreen;
    Model* sphere;
    Entity* sphere1;
    SSAO& ssao;
    Renderer& renderer;
    EditorLayer::Editor& editor;
    Spawner spawner;

    float songSampleInterval = 1;
    vector<SongSample> songData;
    int songDataIndex = 0;

    float timeToDispense2 = timeToDispense;
    float timeToDispense = songSampleInterval;

    Image* textBack;
    Image* textBack1;
    Image* barCover;

    Button* pauseButton;

    ResizableImage* acceptBar;

public:

    exampleSceneScript(SceneManagement::SceneManager& sm, glm::vec3& lightColor, Shader& shaderPbr, Shader* imageShader, Shader* imageShaderGreen, Model* sphere, Entity* sphere1, SSAO& ssao, Renderer& renderer, EditorLayer::Editor& editor)
            : sm(sm), lightColor(lightColor), shaderPbr(shaderPbr), imageShader(imageShader), imageShaderGreen(imageShaderGreen), sphere(sphere), sphere1(sphere1), ssao(ssao), renderer(renderer), editor(editor), spawner(sm.getLoadedScenes().at(0))
    {
        textBack = nullptr;
        textBack1 = nullptr;
        barCover = nullptr;
        pauseButton = nullptr;
        acceptBar = nullptr;
    }

    void awake() override{};

    void start() override{
        SongAnalizer::parseSong(songSampleInterval, "res/content/sounds/queen.wav", songData);

        Texture textBac("res/content/textures/backgg.jpg", "backgg");
        Texture textBac1("res/content/textures/backgg.jpg", "backgg");
        Texture barCov("res/content/textures/dupa.png", "dupa");
        Texture pauseImg("res/content/textures/stop.png", "stop");

        textBack = new Image(imageShader);
        textBack->setTexture(&textBac);

        textBack1 = new Image(imageShader);
        textBack1 -> setTexture(&textBac);

        barCover = new Image(imageShader);
        barCover -> setTexture(&barCov);

        pauseButton = new Button(imageShader);
        pauseButton -> setTexture(&pauseImg);
        pauseButton->setOnClick([]() {
            std::cout << "pauza" << std::endl;
        });

        acceptBar = new ResizableImage(imageShaderGreen);

        //Image
        Entity* textBG = new Entity("textBack");
        sm.getLoadedScenes()[0]->addEntity(textBG);
        textBG->addComponent(textBack);
        textBack->getTransform()->setScale(glm::vec3(0.15f, 0.15f, 0.0f));
        textBack->getTransform()->setPosition(glm::vec3(-0.8f, 0.75f, 0.0f));

        Entity* textBG1 = new Entity("textBack1");
        sm.getLoadedScenes()[0]->addEntity(textBG1);
        textBG1->addComponent(textBack1);
        textBack1->getTransform()->setScale(glm::vec3(0.15f, 0.15f, 0.0f));
        textBack1->getTransform()->setPosition(glm::vec3(0.0f, 0.75f, 0.0f));

        Entity* barImageCover = new Entity("barCover");
        sm.getLoadedScenes()[0]->addEntity(barImageCover);
        barImageCover->addComponent(barCover);
        barCover->getTransform()->setScale(glm::vec3(0.030f, 0.330f, 0.0f));
        barCover->getTransform()->setPosition(glm::vec3(0.9f, 0.068f, 0.0f));

        //Button
        Entity* stopButton = new Entity("pauseButton");
        sm.getLoadedScenes()[0]->addEntity(stopButton);
        stopButton->addComponent(pauseButton);
        pauseButton->getTransform()->setScale(glm::vec3(0.04f, 0.05f, 0.05f));
        pauseButton->getTransform()->setPosition(glm::vec3(0.9f, 0.75f, 0.0f));

        //ResizableImage
        Entity* accBar = new Entity("acceptBar");
        sm.getLoadedScenes()[0]->addEntity(accBar);
        accBar->addComponent(acceptBar);
        acceptBar->getTransform()->setScale(glm::vec3(0.015f, 0.3f, 0.0f));
        acceptBar->getTransform()->setPosition(glm::vec3(0.905f, 0.05f, 0.0f));
    };

    void update(const glm::mat4& projection, const glm::mat4& view) override{
        if (timeToDispense2 < 0 && songDataIndex < songData.size())
        {
            //spawner.spawnBall("bass", glm::vec3(-songData[songDataIndex].bass.x, -songData[songDataIndex].bass.y, -20));
            //spawner.spawnBall("mid", glm::vec3(-songData[songDataIndex].mid.x, songData[songDataIndex].mid.y, -20));
            //spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, songData[songDataIndex].high.y, -20));
            //spawner.spawnBall("high", glm::vec3(songData[songDataIndex].high.x, -songData[songDataIndex].high.y, -20));

            songDataIndex++;
            timeToDispense2 = timeToDispense;
        }

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
        // send light relevant uniforms
        glm::vec3 lightPosView = glm::vec3(s.camera.GetViewMatrix() * glm::vec4(sphere1->getTransform()->getLocalPosition(), 1.0));
        ssao.shaderLightingPass.setVec3("light.Position", lightPosView);
        ssao.shaderLightingPass.setVec3("light.Color", lightColor);
        // Update attenuation parameters


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
        //scene.update();


        imageShader->use();
        glDisable(GL_DEPTH_TEST);
        //imageShader->use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //kolejnosc renderowania ma znaczenie
        //pierwszy renderowany obiekt bedzie pod spodem
        //1

        //2
        textBack->renderPlane();
        textBack1->renderPlane();

        //3
        pauseButton->renderPlane();

        acceptBar->renderPlane();

        // Zmienna do przechowywania czasu od ostatniego wywołania resizeOnImpulse
        static float timeSinceLastResize = 0.0f;

        // Oblicz czas od ostatniej klatki (deltaTime)
        float deltaTime = glfwGetTime() - timeSinceLastResize;

        // Jeśli minęła sekunda od ostatniego wywołania resizeOnImpulse
        if (deltaTime >= 1.0f) {
            // Wywołaj resizeOnImpulse
            acceptBar->resizeOnImpulse(0.01f);

            // Zresetuj licznik czasu
            timeSinceLastResize = glfwGetTime();
        }

        barCover->renderPlane();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        //TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////
        double mouseX, mouseY;
        glfwGetCursorPos(s.window, &mouseX, &mouseY);

        pauseButton->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        pauseButton->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);


        //TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////
    };

    void onDestroy() override{};

    ~exampleSceneScript() override = default;
};

#endif
