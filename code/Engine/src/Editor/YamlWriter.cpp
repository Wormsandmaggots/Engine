//
// Created by Radek on 08.04.2024.
//

#include <fstream>
#include "Editor/YamlWriter.h"

void YamlWriter::WriteTo(std::string path, YAML::Node toWrite) {
    std::ofstream file(path);
}
