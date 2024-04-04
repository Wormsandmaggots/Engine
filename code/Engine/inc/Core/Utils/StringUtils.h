//
// Created by Radek on 04.04.2024.
//

#ifndef ENGINE_STRINGUTILS_H
#define ENGINE_STRINGUTILS_H

#include "string"

namespace utils {
    inline int stringToHash(const std::string &name) {
        return (int) std::hash<std::string>{}(name);
    }
}

#endif //ENGINE_STRINGUTILS_H
