//
// Created by Radek on 08.04.2024.
//

#ifndef ENGINE_YAMLWRITER_H
#define ENGINE_YAMLWRITER_H

#include <string>
#include "yaml-cpp/node/node.h"

class Scene2;

class YamlWriter {
public:
    static void WriteTo(std::string path, YAML::Node toWrite);
    static void WriteTo(const Scene2&);
    static void WriteTo(Scene2*, std::string);
private:
    static std::string lastlySavedScene;
};


#endif //ENGINE_YAMLWRITER_H
