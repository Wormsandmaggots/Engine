#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"

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
#include "Renderer/Renderer.h"

//CODE FROM BELOW SHOULD GO TO THEIR CORRESPONDING FILES IS USEFUL
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

//float speed = 5;
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

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        s.camera.ProcessKeyboard(FORWARD, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        s.camera.ProcessKeyboard(BACKWARD, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        s.camera.ProcessKeyboard(LEFT, s.deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        s.camera.ProcessKeyboard(RIGHT, s.deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        s.camera.ProcessKeyboard(UP, s.deltaTime);
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        s.camera.ProcessKeyboard(DOWN, s.deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (s.firstMouse)
    {
        s.lastX = xpos;
        s.lastY = ypos;
        s.firstMouse = false;
    }

    float xoffset = xpos - s.lastX;
    float yoffset = s.lastY - ypos; // reversed since y-coordinates go from bottom to top

    s.lastX = xpos;
    s.lastY = ypos;

    s.camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

    s.camera.canMove = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //if(carMovement) return;

    s.camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void SetCallbacks(GLFWwindow* window)
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void imgui_begin()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void init(){
    //GLFW INITIALIZATION
    if (GLFWInit())
    {
        LOG_ERROR("Failed to initialize GLFW");
        exit(-1);
    }
    LOG_INFO("GLFW initialized");

    //CREATING WINDOW
    s.window = glfwCreateWindow(s.WINDOW_WIDTH, s.WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    glEnable(GL_DEPTH_TEST); //enabling deoth tests
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //determines how OpenGL renders the edges and interiors of polygons
    SetCallbacks(s.window);//setting the callback functions for the window

    //INITIALIZING IMGUI
    init_imgui();

    //INITIALIZING PROFILER
    #if defined(PROFILER)
        Profiler::get().init();
    #endif
}
void end(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(s.window);
    glfwTerminate();
}
#endif //ENGINE_ENGINE_H