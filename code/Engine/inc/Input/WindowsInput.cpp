#include "WindowsInput.h"

Input* Input::s_Instance = nullptr;

WindowsInput::WindowsInput(GLFWwindow* window) : window(window) {}

bool WindowsInput::IsKeyPressedImpl(int keycode)
{
    auto state = glfwGetKey(window , keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button) {
    auto state = glfwGetKey(window , button);
    return state == GLFW_PRESS;
}

float WindowsInput::GetMouseXImpl() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return (float)xpos;
}

float WindowsInput::GetMouseYImpl() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return (float)ypos;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return {(float) xpos, (float) ypos };
}
