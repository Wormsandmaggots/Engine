#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include "Core/AssetManager/AssetManager.h"
#include "Debug/Profiler.h"
#include "tracy/Tracy.hpp"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
//#define PROFILER
#if defined(PROFILER) //overloading operators new and delete globally for profiling
void* operator new(std::size_t count)
{
    auto ptr = malloc(count);
    TracyAlloc(ptr, count);
    return ptr;
}
void operator delete(void* ptr) noexcept
{
    TracyFree(ptr);
    free(ptr);
}
#endif
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "stb_image.h"
#include "JsonReader.h"
#include "Editor/Camera.h"
#include "Scene/Loader/YamlReader.h"
#include "Scene/Loader/SceneLoader.h"
#include "Scene/SceneManager.h"
#include "Debug/Logger.h"
#include "Audio/AudioManager.h"
#include "Editor/Gizmos.h"
#include "Engine/inc/Physics/Colliders/Collider.h"
#include "Input/Input.h"

struct Settings{
    int32_t WINDOW_WIDTH  = 1920;
    int32_t WINDOW_HEIGHT = 1080;

    GLFWwindow* window = nullptr;

// Change these to lower GL version like 4.5 if GL 4.6 can't be initialized on your machine
    const char* glsl_version = "#version 460";
    int32_t GL_VERSION_MAJOR = 4;
    int32_t GL_VERSION_MINOR = 6;

    std::string jsonSettingsFilePath = "res/content/maps/ExampleNotWorkingScene.json";

    float lastX = WINDOW_WIDTH / 2.0f;
    float lastY = WINDOW_HEIGHT / 2.0f;
    bool firstMouse = true;

    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f;
    Camera camera;
} s;
#pragma region INITIALIZERS
int GLFWInit()
{
    if (!glfwInit())
        return 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, s.GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, s.GL_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    return 0;
}

void init_imgui()
{
    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(s.window, true);
    ImGui_ImplOpenGL3_Init(s.glsl_version);

    ImGui::StyleColorsDark();
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

float speed = 5;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void SetCallbacks(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    Input::getInstance().setupInputCallbacks(window);
}

void imgui_begin()
{
    ZoneTransientN(imgui, "ImguiInit", true);
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}
#pragma endregion INITIALIZERS
///!!!!!----------------------------->
//USE THIS TO GET A TRANSFORM FROM A JSON
JsonReader j(s.jsonSettingsFilePath);
//Transform* CreateTransform(std::string pathToObjectInJson)
//{
//    return new Transform(new Model(j.ParseToString(pathToObjectInJson, "modelPath")),
//                         j.ParseToVec3(pathToObjectInJson, "pos"),
//                         j.ParseToVec3(pathToObjectInJson, "rot"),
//                         j.ParseToVec3(pathToObjectInJson, "scale"));
//}

void init(){
    //GLFW INITIALIZATION
    if (GLFWInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        exit(-1);
    }
    LOG_INFO("GLFW initialized");


    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    // Get the video mode of the monitor
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);


    //CREATING WINDOW
   // s.window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "Just Hero", NULL, NULL);
    //fullscreen
    s.window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "Just Hero", monitor, NULL);
    if (s.window == nullptr)
    {
        LOG_ERROR("Failed to Create GLFW window");
        glfwTerminate();
        exit(-1);
    }
    LOG_INFO("GLFW window created");
    glfwMakeContextCurrent(s.window);
    glfwSwapInterval(1); //how often buffers will swap

    //GLAD INITIALIZING
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_ERROR("Failed to initialize GLAD");
        glfwDestroyWindow(s.window);
        glfwTerminate();
        exit(-1);
    }
    LOG_INFO("GLAD initialized");

    //SETTING OPENGL PARAMETERS
//    glEnable(GL_DEPTH_TEST); //enabling deoth tests
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //determines how OpenGL renders the edges and interiors of polygons
    SetCallbacks(s.window);//setting the callback functions for the window

    //INITIALIZING IMGUI
    init_imgui();

    //INITIALIZING PROFILER
    #if defined(PROFILER)
        Profiler::get().init();
    #endif
}
void update(){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(s.window);
        glfwMakeContextCurrent(s.window);
        glfwPollEvents();
}
void end(){
    AssetManager::end();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(s.window);
    glfwTerminate();
}
#endif //ENGINE_ENGINE_H