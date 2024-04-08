#ifndef ENGINE_WINDOWSINPUT_H
#define ENGINE_WINDOWSINPUT_H
#include "Input.h"
class WindowsInput : public Input{
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
};
#endif //ENGINE_WINDOWSINPUT_H
