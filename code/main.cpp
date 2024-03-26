#include <iostream>
#include "Engine/Engine.h"


int main() {

    if(GLFWInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        return -1;
    }
    LOG_INFO("GLFW initialized");

    //Radek note: don't mind me, just testing
#pragma region Audio
    AudioManager a;
    a.init();

    Sound *sound = a.loadSound("res/content/sounds/Ich will.mp3", "testSound");
    SceneManager sm;

    sm.loadScene("res/content/maps/exampleScene.yaml");
    sound->play();

    //! THEE WHO SHALL FIND THIS VOLUME VALUE,
    //! BE AWARE OF CONSEQUENCES STANDING BEHIND ALTERING IT
    sound->setVolume(2.f);

    LOG_INFO("If u hear germans singing, that's a good sing.");
#pragma endregion Audio

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

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_TRUE); // Enables writing into the depth buffer.
    glDepthRange(0.0f, 1.0f); // Maps normalized z-coordinates to window coordinates.



    init_imgui();

    SetCallbacks(s.window);
Transform* monke = CreateTransform("tire4");
Scene scene;
scene.addObjects(monke);

    Transform* plane = CreateTransform("tire1");
    scene.addObjects(plane);
  

Shader shader("res/content/shaders/vertex.glsl", "res/content/shaders/fragment.glsl");

    while (!glfwWindowShouldClose(s.window)) {
        imgui_begin();
        imgui_render();

        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;

        processInput(s.window);

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        shader.setMat4("view", s.camera.GetViewMatrix());
        glm::mat4 projection = glm::perspective(glm::radians(s.camera.Zoom), (float)s.WINDOW_WIDTH / (float)s.WINDOW_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("projection", projection);
        scene.UpdateTransform(shader);

//        sm.updateLoadedScenes();

//        Gizmos::DrawGizmos(s.camera.GetViewMatrix(), projection, *monke->model);
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        imgui_end();
        glfwSwapBuffers(s.window);
        glfwMakeContextCurrent(s.window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(s.window);
    glfwTerminate();

    return 0;
}
