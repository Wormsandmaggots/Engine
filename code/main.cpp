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
#include "HUD/Button.h"
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
    //for visual debugging
    Shader imageShaderGreen("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_green.glsl");
    Shader imageShaderBlue("res/content/shaders/vertex_2d.glsl", "res/content/shaders/fragment_2d_blue.glsl");


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

    renderer.addShader(&imageShader);
    renderer.addShader(&imageShaderGreen);
    renderer.addShader(&imageShaderBlue);


    Text* arcadeRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");
    Text* counterRenderer = new Text("res/content/fonts/ARCADECLASSIC.TTF");

    arcadeRenderer->setParameters("dupa", 100, 100, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), (float) s.WINDOW_WIDTH,(float) s.WINDOW_HEIGHT);
    ThirdPersonCamera* playerCamera = new ThirdPersonCamera();
    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);




    Image* menuBackground = new Image(&imageShader);

    Image* menuPole = new Image(&imageShader);



    Button* newGame = new Button(&imageShader);
    newGame->setOnClick([]() {
        std::cout << "start game" << std::endl;
    });

    Button* exit = new Button(&imageShader);
    exit->setOnClick([]() {
        std::cout << "wyjscie" << std::endl;
    });



    //main menu
    //Image
    Entity* menuWalpaper = new Entity("menuBackground");
    sm.getLoadedScenes()[0]->addEntity(menuWalpaper);
    menuWalpaper->addComponent(menuBackground);
    menuBackground->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    menuBackground->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    Entity* menuAditional = new Entity("menuPole");
    sm.getLoadedScenes()[0]->addEntity(menuAditional);
    menuAditional->addComponent(menuPole);
    menuPole->getTransform()->setScale(glm::vec3(1.0f, 1.0f, 1.0f));
    menuPole->getTransform()->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

    //button
    Entity* ng = new Entity("newGame");
    sm.getLoadedScenes()[0]->addEntity(ng);
    ng->addComponent(newGame);
    newGame->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    newGame->getTransform()->setPosition(glm::vec3(-0.75f, -0.1f, 0.0f));

    Entity* ex = new Entity("exit");
    sm.getLoadedScenes()[0]->addEntity(ex);
    ex->addComponent(exit);
    exit->getTransform()->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    exit->getTransform()->setPosition(glm::vec3(-0.75f, -0.3f, 0.0f));

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


        renderer.updateProjectionAndView(projection, view);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        editor.draw();

        sm.updateLoadedScenes();



        //HUD Menu
        glDisable(GL_DEPTH_TEST);
        imageShader.use();
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //kolejnosc renderowania ma znaczenie
        //pierwszy renderowany obiekt bedzie pod spodem
        //1
        menuBackground->setTexture("res/content/textures/backgg.jpg");
        menuBackground->renderPlane();
        menuPole->setTexture("res/content/textures/pole.png");
        menuPole->renderPlane();

        //2
        newGame->setTexture("res/content/textures/ng.png");
        newGame->renderPlane();

        exit->setTexture("res/content/textures/ex.png");
        exit->renderPlane();

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);



        //TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////
        double mouseX, mouseY;
        glfwGetCursorPos(s.window, &mouseX, &mouseY);

        newGame->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        newGame->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);

        exit->checkHover(mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        exit->checkClick(s.window, mouseX, mouseY, s.WINDOW_WIDTH, s.WINDOW_HEIGHT);
        //TEMPORARY SOLUTION - TO BE SWITCH FOR CONTROLER INPUT ///////////////////////


        cm.update();

        update();
    }
    a.end();

    end();
    return 0;
}