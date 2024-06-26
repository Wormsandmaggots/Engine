#include "Engine/Engine.h"
#include "Renderer/FrameBuffer.h"
#include "Globals.h"
#include "Input/PlayerInput.h"
#include "Script/exampleSceneScript.h"
#include <thread>

using namespace SceneManagement;

int main() {

    init();
    glViewport(0,0, 1920, 1080);

    //instance

    //awake
    bool *isLoading = new bool(true);

    Shader sh = Shader("res/content/shaders/framebuffer.vert", "res/content/shaders/framebuffer.frag");


    std::thread worker([](bool *load, GLFWwindow* w, Shader* sh) {

        //CREATING WINDOW
        GLFWwindow* window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "Loading...", NULL, w);
        if (window == nullptr)
        {
            LOG_ERROR("Failed to Create GLFW window");
            glfwTerminate();
            exit(-1);
        }

        LOG_INFO("GLFW window created");
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); //how often buffers will swap

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            LOG_ERROR("Failed to initialize GLAD in worker thread");
            glfwDestroyWindow(window);
            glfwTerminate();
            return;
        }

        float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
                // positions   // texCoords
                -1.0f,  1.0f,  0.0f, 1.0f,
                -1.0f, -1.0f,  0.0f, 0.0f,
                1.0f, -1.0f,  1.0f, 0.0f,

                -1.0f,  1.0f,  0.0f, 1.0f,
                1.0f, -1.0f,  1.0f, 0.0f,
                1.0f,  1.0f,  1.0f, 1.0f
        };

        unsigned int quadVAO, quadVBO;

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        Texture loadTexture = Texture("res/content/images.jpg", "load");


        sh->use();
        sh->setInt("screenTexture", 0);


        while(*load == true)
        {
            glClearColor(0.8, 0.8, 0.8, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            sh->use();
            glActiveTexture(GL_TEXTURE0);
            loadTexture.bind();
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
            glfwMakeContextCurrent(window);
            glfwPollEvents();
        }

    }, isLoading, s.window, &sh);

    worker.detach();

    glfwHideWindow(s.window);

    exampleSceneScript* examplesceneScript = new exampleSceneScript();
    examplesceneScript->awake();
    examplesceneScript->start();

    *isLoading = false;

    glfwShowWindow(s.window);

    glfwMakeContextCurrent(s.window);

    while (!glfwWindowShouldClose(s.window))
    {
        examplesceneScript->update();

        update();
    }

    examplesceneScript->onDestroy();


    end();
    return 0;
}