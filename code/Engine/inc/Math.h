//
// Created by Radek on 25.03.2024.
//

#ifndef ENGINE_MATH_H
#define ENGINE_MATH_H

template<typename T>
constexpr void clamp(T &toClamp, T low, T high)
{
    toClamp = toClamp < low ? low : toClamp > high ? high : low;
}

template<typename T, typename Compare>
constexpr void clamp(T toClamp, T low, T high, Compare compare)
{
    toClamp = compare(toClamp, low) < 0 ? low : compare(toClamp, high) > 0 ? high : low;
}

//!THIS IS COMPARE METHOD EXAMPLE
//!IF A > B return value bigger than 0
//!IF A < B return value lesser than 0
//!ELSE return 0
//template <typename T>
//int compare(const T& a, const T& b) {
//    if (a < b) {
//        return -1;
//    } else if (b < a) {
//        return 1;
//    } else {
//        return 0;
//    }
//}

#endif //ENGINE_MATH_H
