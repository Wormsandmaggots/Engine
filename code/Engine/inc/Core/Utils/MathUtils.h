//
// Created by Radek on 04.04.2024.
//

#ifndef ENGINE_MATHUTILS_H
#define ENGINE_MATHUTILS_H

#include "glm/geometric.hpp"

class Math {
public:
    template<typename T>
    static constexpr T SafeNormal(const T &x) {
        return glm::length(x) == 0.f ? T{0.f} : glm::normalize(x);
    }

    template<typename T>
    static constexpr void Clamp(T &toClamp, T low, T high) {
        toClamp = toClamp < low ? low : toClamp > high ? high : low;
    }

    template<typename T, typename Compare>
    static constexpr void Clamp(T toClamp, T low, T high, Compare compare) {
        toClamp = compare(toClamp, low) < 0 ? low : compare(toClamp, high) > 0 ? high : low;
    }
};
#endif //ENGINE_MATHUTILS_H
