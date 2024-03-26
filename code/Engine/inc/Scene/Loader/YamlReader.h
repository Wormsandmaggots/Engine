//
// Created by Radek on 23.03.2024.
//

#ifndef ENGINE_YAMLREADER_H
#define ENGINE_YAMLREADER_H

#include <string>
#include "yaml-cpp/yaml.h"
#include "Reader.h"
#include "Debug/Logger.h"

class YamlReader : public Reader{
public:
    YamlReader() = default;
    ~YamlReader() override = default;

    int parseFile(std::string path) override;
    YAML::Node getData() const;

    template<class T>
    std::optional<T> getByPath(std::initializer_list<std::string> pathToValue) const{
        if(data.IsNull())
        {
            LOG_ERROR("Before searching value, parse the file.\n");

            return std::nullopt;
        }

        YAML::Node currentNode = data;

        for (std::string key : pathToValue) {
            if(!currentNode[key])
            {
                LOG_ERROR("I have not found a node '" + key + "' in provided path.\n");

                return std::nullopt;
            }

            currentNode = currentNode[key];
        }

        return currentNode.as<T>();
    }

private:
    YAML::Node data;
};


#endif //ENGINE_YAMLREADER_H
