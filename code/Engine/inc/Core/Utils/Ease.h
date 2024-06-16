//
// Created by Radek on 05.06.2024.
//

#ifndef ENGINE_EASE_H
#define ENGINE_EASE_H
#include <cmath>

namespace utils
{
    float easeInOutQuint(float x) {
        return x < 0.5 ? 16 * x * x * x * x * x : 1 - pow(-2 * x + 2, 5) / 2;
    }

    float easeInCubic(float x) {
        return x * x * x;
    }

    float easeInCirc(float x) {
        return 1 - sqrt(1 - pow(x, 2));
    }

    float easeInOutExpo(float x) {
    return x == 0
    ? 0
    : x == 1
    ? 1
    : x < 0.5 ? pow(2, 20 * x - 10) / 2
    : (2 - pow(2, -20 * x + 10)) / 2;
    }
}

#endif //ENGINE_EASE_H
