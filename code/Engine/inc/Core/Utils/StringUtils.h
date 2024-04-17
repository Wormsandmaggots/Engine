//
// Created by Radek on 04.04.2024.
//

#ifndef ENGINE_STRINGUTILS_H
#define ENGINE_STRINGUTILS_H

#include "string"
#include "glm/glm.hpp"

namespace utils {
    static inline int stringToHash(const std::string &name) {
        return (int) std::hash<std::string>{}(name);
    }

    static std::string mat4ToString(const glm::mat4 &matrix) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(4); // Ustawienie precyzji do 4 miejsc po przecinku

        // Iteruj po każdym elemencie macierzy
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                oss << matrix[i][j]; // Dodanie elementu do strumienia
                if (j < 3) {
                    oss << " "; // Dodanie spacji poza ostatnim elementem w wierszu
                }
            }
            if (i < 3) {
                oss << "\n"; // Dodanie nowej linii poza ostatnim wierszem
            }
        }

        return oss.str(); // Zwrócenie ciągu znaków
    }
}

#endif //ENGINE_STRINGUTILS_H
