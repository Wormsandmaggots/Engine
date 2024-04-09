#include "Input.h"

void Input::set_input(const std::shared_ptr<Input> &input_system) {
    input = input_system;

    glfwSetCursorPosCallback(input->m_window->get_glfw_window(), &input->mouse_callback);
    glfwSetWindowFocusCallback(input->m_window->get_glfw_window(), &input->focus_callback);
}
