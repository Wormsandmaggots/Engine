#include <iostream>
#include "Engine/Engine.h"  


int main() {

    if(GLFWInit())
    {
        ERRORLOG("Failed to initialize GLFW");
        return -1;
    }

    INFOLOG("GLFW initialized");

    s.window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (s.window == nullptr)
    {
        ERRORLOG("Failed to Create GLFW window");
        glfwTerminate();
        return -1;
    }

    INFOLOG("GLFW window created");
    glfwMakeContextCurrent(s.window);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ERRORLOG("Failed to initialize GLAD");

        glfwDestroyWindow(s.window);
        glfwTerminate();

        return -1;
    }

    INFOLOG("GLAD initialized");

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    init_imgui();

    while (!glfwWindowShouldClose(s.window)) {

        float currentFrame = static_cast<float>(glfwGetTime());
        s.deltaTime = currentFrame - s.lastFrame;
        s.lastFrame = currentFrame;

        //processInput(s.window);

        glClearColor(0.2, 0.2, 0.2, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
