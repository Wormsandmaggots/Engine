#ifndef ENGINE_WINDOWSINPUT_H
#define ENGINE_WINDOWSINPUT_H

#include <utility>
#include "GLFW/glfw3.h"
#include "Input.h"

class WindowsInput : public Input{
public:
    WindowsInput(GLFWwindow* window);
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;

    virtual bool IsMouseButtonPressedImpl(int button) override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
private:
    GLFWwindow* window;
};

#endif
