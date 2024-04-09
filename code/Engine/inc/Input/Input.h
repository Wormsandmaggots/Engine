#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H
#include <map>

#include "Event.h"
#include "Key.h"
#include "Window.h"

class Input {
public:
    static void set_input(std::shared_ptr<Input> const &input_system);

    inline static std::shared_ptr<Input> input;

private:

    std::map<i32, Key> m_keys = {};

    std::shared_ptr<Window> m_window;
};

#endif //ENGINE_INPUT_H