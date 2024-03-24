//
// Created by Radek on 23.03.2024.
//

#include <iostream>
#include "Scene/Loader/YamlReader.h"

//overrides previously cached data
int YamlReader::parseFile(std::string path) {
    if(filename == path) return 0;

    filename = path;
    try {
        data = YAML::LoadFile(filename);
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Encountered problem with yaml file: " + path + "\n" + e.what());
    }

    return 0;
}

YAML::Node YamlReader::getData() const {
    return data;
}
