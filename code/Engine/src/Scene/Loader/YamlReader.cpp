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

template<class T>
std::optional<T> YamlReader::getByPath(const YAML::Node& start, std::initializer_list<std::string> pathToValue) const {
    if(start.IsNull())
    {
        LOG_ERROR("Before searching value, parse the file.\n");

        return std::nullopt;
    }

    YAML::Node currentNode = start;

    for (const std::string& key : pathToValue) {
        if(!currentNode[key])
        {
            LOG_ERROR("I have not found a node '" + key + "' in provided path.\n");

            return std::nullopt;
        }

        currentNode = currentNode[key];
    }
    return currentNode.as<T>();
}
