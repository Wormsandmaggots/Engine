#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H
class Input {
public:
    static bool IsKeyPressed(int keycode) {return s_Instance->IsKeyPressedImpl(keycode);}
protected:
    virtual bool IsKeyPressedImpl(int keycode) = 0;
private:
    static Input* s_Instance;
};
#endif //ENGINE_INPUT_H