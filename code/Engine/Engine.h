#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H


#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "Transform_old.h"
#include "JsonReader.h"
#include "Camera.h"
#include "Engine/inc/Scene.h"
#include "Scene/Loader/YamlReader.h"
#include "Scene/Loader/SceneLoader.h"
#include "Scene/SceneManager.h"
#include "Debug/Logger.h"
#include "Audio/AudioManager.h"
#include "Debug/Gizmos.h"

struct Settings {
    int32_t WINDOW_WIDTH = 1920;
    int32_t WINDOW_HEIGHT = 1080;

    GLFWwindow *window = nullptr;

    const char *glsl_version = "#version 460";
    int32_t GL_VERSION_MAJOR = 4;
    int32_t GL_VERSION_MINOR = 6;

    std::string jsonSettingsFilePath = "res/content/maps/ExampleNotWorkingScene.json";

    float lastX = WINDOW_WIDTH / 2.0f;
    float lastY = WINDOW_HEIGHT / 2.0f;
    bool firstMouse = true;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    Camera camera;
} s;

static void glfw_error_callback(int error, const char *description) {
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int GLFWInit() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, s.GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, s.GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    return 0;
}


///!!!!!----------------------------->
//USE THIS TO GET A TRANSFORM FROM A JSON
JsonReader j(s.jsonSettingsFilePath);

Transform_old *CreateTransform(std::string pathToObjectInJson) {
    return new Transform_old(new Model(j.ParseToString(pathToObjectInJson, "modelPath")),
                             j.ParseToVec3(pathToObjectInJson, "pos"),
                             j.ParseToVec3(pathToObjectInJson, "rot"),
                             j.ParseToVec3(pathToObjectInJson, "scale"));
}


float speed = 5;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //if(carMovement) return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        s.camera.ProcessKeyboard(FORWARD, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

        s.camera.ProcessKeyboard(BACKWARD, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        s.camera.ProcessKeyboard(LEFT, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

        s.camera.ProcessKeyboard(RIGHT, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        s.camera.ProcessKeyboard(UP, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        s.camera.ProcessKeyboard(DOWN, s.deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (s.firstMouse) {
        s.lastX = xpos;
        s.lastY = ypos;
        s.firstMouse = false;
    }

    float xoffset = xpos - s.lastX;
    float yoffset = s.lastY - ypos;

    s.lastX = xpos;
    s.lastY = ypos;

    s.camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {

    s.camera.canMove = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    s.camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void SetCallbacks(GLFWwindow *window) {
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
//    glfwSetMouseButtonCallback(window, mouse_button_callback);
}


void imgui_render() {

    ImGuiIO &io = ImGui::GetIO();
    double x, y;
    glfwGetCursorPos(s.window, &x, &y);
    io.MousePos = ImVec2((float) x, (float) y);
    ImGui::Begin("Test window");
    {
        ImGui::Text("doopa");
        ImGui::Button("Buton");
    }
    ImGui::End();

}

void imgui_begin() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void imgui_end() {
    ImGui::Render();
    int display_w, display_h;
    glfwMakeContextCurrent(s.window);
    glfwGetFramebufferSize(s.window, &display_w, &display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void init_imgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // >>>>Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos; // Enable SetMousePos()
    io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen; // Enable SetMousePos()

    ImGui_ImplGlfw_InitForOpenGL(s.window, true);
    ImGui_ImplOpenGL3_Init(s.glsl_version);

    ImGui::StyleColorsDark();

}

#endif //ENGINE_ENGINE_H
