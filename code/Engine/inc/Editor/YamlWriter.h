//
// Created by Radek on 08.04.2024.
//

#ifndef ENGINE_YAMLWRITER_H
#define ENGINE_YAMLWRITER_H

#include <string>
#include "yaml-cpp/node/node.h"

class YamlWriter {
public:
    static void WriteTo(std::string path, YAML::Node toWrite);
private:

};


#endif //ENGINE_YAMLWRITER_H
